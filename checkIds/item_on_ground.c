#include <object.h>
#include <room.h>
#include <item.h>
#include <itemMetaData.h>
#include "base.h"

extern Entity* GetEmptyEntityByKind(u32 kind);
extern void RegisterRoomEntity(Entity*, const EntityData*);
extern void sub_0804AF0C(Entity*, const EntityData*);

Entity* LoadRoomEntity(const EntityData* dat) {
    int kind;
    Entity* entity;

    kind = dat->kind & 0xF;
    if ((dat->flags & 0xF0) == 0x50 && DeepFindEntityByID(kind, dat->id))
        return NULL;
    entity = GetEmptyEntityByKind(kind);
    if (entity != NULL) {
        int item = 0;
        entity->kind = kind;
        entity->id = dat->id;
        if(entity->kind == OBJECT && entity->id == GROUND_ITEM)
        {
            int flag = flag2global(dat->spritePtr >> 16);
            if (flag == -1)
            {
                entity->type = dat->type;
            }
            else
            {
                int item = peek_item_for_global_flag(flag);
                entity->type = get_item_id(item);
            }
        }
        else
        {
            entity->type = dat->type;
        }
        RegisterRoomEntity(entity, dat);
        if ((dat->flags & 0xF0) != 16) {
            u8 kind2;
            if(entity->kind == OBJECT && entity->id == GROUND_ITEM && item!=0)
            {
                entity->type2 = get_item_subvalue(item);
            }
            else
            {
                entity->type2 = *(u8*)&dat->type2;
            }
            entity->type2 = *(u8*)&dat->type2;
            entity->timer = (dat->type2 & 0xFF00) >> 8;
            if (kind == 9)
                return entity;
            sub_0804AF0C(entity, dat);
            if (!entity->next)
                return entity;
            kind2 = dat->kind & 0xF0;
            if ((kind2 & 0x10) == 0) {
                if ((kind2 & 0x20) != 0) {
                    entity->collisionLayer = 2;
                    return entity;
                }
            }

            if ((kind2 & 0x10) || (gRoomControls.scroll_flags & 2)) {
                entity->collisionLayer = 1;
                return entity;
            }

            ResolveCollisionLayer(entity);
            return entity;
        }
    }
    return entity;
}

extern u32 GiveItem(u32, u32);

bool32 sub_08081420(Entity* this) {
    int flag = flag2global(this->field_0x86.HWORD);
    int item_id = this->type;
    int subvalue = this->type2;
    if (flag != -1)
    {
        int item = get_item_for_global_flag(flag);
        item_id = get_item_id(item);
        subvalue = get_item_subvalue(item);
    }

    if (((gItemMetaData[item_id].unk3 & 0x2) || !GetInventoryValue(item_id))) {
        SetDefaultPriority(this, PRIO_PLAYER_EVENT);
        CreateItemEntity(item_id, subvalue, 0);
        return TRUE;
    } else {
        GiveItem(item_id, subvalue);
        return FALSE;
    }
}

void sub_08081404(Entity* this, u32 arg1) {
    if(this->id == GROUND_ITEM){
        if (arg1 && this->field_0x86.HWORD) {
            int flag = flag2global(this->field_0x86.HWORD);
            if (flag == -1)
                SetFlag(this->field_0x86.HWORD);
            //else
                //set_item_global_flag(flag);
        }
    }else{
        if (arg1 && this->field_0x86.HWORD) {
            SetFlag(this->field_0x86.HWORD);
        }
    }

    DeleteThisEntity();
}
