#include <global.h>
#include <area.h>

int peek_item_for_global_flag(int flag);
int get_item_for_global_flag(int flag);

inline int get_item_id(int item)
{
    return item & 0xff;
}

inline int get_item_subvalue(int item)
{
    return (item >> 8) & 0xff;
}

inline int local2global(int flag)
{
    return gArea.localFlagOffset + flag;
}

inline int flag2global(int flag)
{
    u32 type;
    u32 index;

    if (flag != 0) {
        index = flag & 0x3ff;
        type = (flag & 0xc000) >> 0xe;
        switch (type) {
            case 2:
                // room flag
                return -1;
            case 0:
                // local flag
                return local2global(index);
            case 1:
                // global flag
                return index;
        }
    }
    return -1;
}
