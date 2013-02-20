
//****************************************************************************
//**
//**    mmngr_virtual.cpp
//**		-Virtual Memory Manager
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include <string.h>
#include "mmngr_virtual.h"
#include "mmngr_phys.h"

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

//! page table represents 4mb address space
#define PTABLE_ADDR_SPACE_SIZE 0x400000

//! directory table represents 4gb address space
#define DTABLE_ADDR_SPACE_SIZE 0x100000000

//! page sizes are 4k
#define PAGE_SIZE 4096

//============================================================================
//    IMPLEMENTATION PRIVATE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================

//! current directory table
pdirectory*		_cur_directory=0;

//! current page directory base register
physical_addr	_cur_pdbr=0;

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

inline uint32_t vmmngr_ptable_virt_to_index (virtual_addr addr) {

	//! return index only if address doesnt exceed page table address space size
	return (addr >= PTABLE_ADDR_SPACE_SIZE) ? 0 : addr/PAGE_SIZE;
}

inline pt_entry* vmmngr_ptable_lookup_entry (ptable* p,virtual_addr addr) {

	if (p)
		return &p->m_entries[ vmmngr_ptable_virt_to_index (addr) ];
	return 0;
}

inline void vmmngr_ptable_clear (ptable* p) {

	if (p)
		memset ( p,0,sizeof (ptable) );
}

inline void vmmngr_pdirectory_clear (pdirectory* dir) {

	if (dir)
		memset ( dir,0,sizeof (pdirectory) );
}

inline uint32_t vmmngr_pdirectory_virt_to_index (virtual_addr addr) {

	//! return index only if address doesnt exceed 4gb (page directory address space size)
	return (addr >= DTABLE_ADDR_SPACE_SIZE) ? 0 : addr/PAGE_SIZE;
}

inline pd_entry* vmmngr_pdirectory_lookup_entry (pdirectory* p, virtual_addr addr) {

	if (p)
		return &p->m_entries[ vmmngr_ptable_virt_to_index (addr) ];
	return 0;
}

inline bool vmmngr_switch_pdirectory (pdirectory* dir) {

	if (!dir)
		return false;

	_cur_directory = dir;
	pmmngr_load_PDBR (_cur_pdbr);
	return true;
}

void vmmngr_flush_tlb_entry (virtual_addr addr) {

#ifdef _MSC_VER
	_asm {
		cli
		invlpg	addr
		sti
	}
#endif
}

pdirectory* vmmngr_get_directory () {

	return _cur_directory;
}

bool vmmngr_alloc_page (pt_entry* e) {

	//! allocate a free physical frame
	void* p = pmmngr_alloc_block ();
	if (!p)
		return false;

	//! map it to the page
	pt_entry_set_frame (e, (physical_addr)p);
	pt_entry_add_attrib (e, I86_PTE_PRESENT);

	return true;
}

void vmmngr_free_page (pt_entry* e) {

	void* p = (void*)pt_entry_pfn (*e);
	if (p)
		pmmngr_free_block (p);

	pt_entry_del_attrib (e, I86_PTE_PRESENT);
}

void vmmngr_initialize () {

	//! allocate default page table
	ptable* table = (ptable*) pmmngr_alloc_block ();
	if (!table)
		return;

	//! clear page table
	vmmngr_ptable_clear (table);

	//! idenitity map the page table (First 4mb of virtual memory mapped to same phys address)
	for (int i=0, frame=0; i<1024; i++, frame+=4096) {

		//! create a new page
		pt_entry page=0;
		pt_entry_add_attrib (&page, I86_PTE_PRESENT);
		pt_entry_add_attrib (&page, I86_PTE_USER);
		pt_entry_set_frame (&page, frame);

		//! ...and add it to the page table
		table->m_entries [vmmngr_ptable_virt_to_index (frame) ] = page;
	}

	//! create default directory table
	pdirectory*	dir = (pdirectory*) pmmngr_alloc_blocks (3);
	if (!dir)
		return;

	//! clear directory table and set it as current
	vmmngr_pdirectory_clear (dir);

	//! get first entry in dir table and set it up to point to our table
	pd_entry* entry = vmmngr_pdirectory_lookup_entry (dir,0);
	pd_entry_add_attrib (entry, I86_PDE_PRESENT);
	pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
	pt_entry_add_attrib (entry, I86_PDE_USER);
	pd_entry_set_frame (entry, (physical_addr)table);

	//! store current PDBR
	_cur_pdbr = (physical_addr) &dir->m_entries;

	//! switch to our page directory
	vmmngr_switch_pdirectory (dir);

	//! enable paging
	pmmngr_paging_enable (true);
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[mmngr_virtual.cpp]
//**
//****************************************************************************
