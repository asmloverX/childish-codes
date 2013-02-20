// sl_temp_test_hash_map.cc - Testing hash_map
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/10 09:21:38 by ASMlover.
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <hash_map>
#include <vector>

#include "sl_temp_test.h"


namespace sl_temp {

    class HMClass {
        std::vector<int>* v_;
    public:
        HMClass(void)
            : v_(NULL)
        {}

        ~HMClass(void)
        {}
    public:
        bool init(void)
        {
            if (NULL == v_)
                v_ = new std::vector<int>;
            return (NULL != v_);
        }

        void uninit(void)
        {
            if (NULL != v_) {
                v_->clear();
                delete v_;
                v_ = NULL;
            }
        }

        bool add(int v)
        {
            if (NULL != v_) {
                v_->push_back(v);
                return true;
            }
            return false;
        }

        void show(void)
        {
            std::vector<int>::iterator i;

            if (NULL != v_) {
                for (i = v_->begin(); i != v_->end(); ++i)
                    fprintf(stdout, "%d\t", *i);
                fprintf(stdout, "\n");
            }
        }
    };

    void 
    sl_temp_hash_map(void)
    {
        int count = 0;
        stdext::hash_map<int, HMClass> hmcMap;
        stdext::hash_map<int, HMClass>::iterator i;

        hmcMap.clear();

        srand((unsigned int)time(0));
        while (count++ < 5) {
            if (hmcMap.end() == hmcMap.find(12)) {
                HMClass hmc;

                hmc.init();
                hmc.add(1);
                hmcMap[12] = hmc;
            } else {
                hmcMap[12].add(rand() % 100);
            }
        }

        for (i = hmcMap.begin(); i != hmcMap.end();) {
            i->second.show();
            i->second.uninit();
            i = hmcMap.erase(i);
        }
    }

}