#pragma once

#include "exports_classes/blackbox.h"
#include "exports_classes/common.h"
#include "exports_classes/core.h"
#include "exports_classes/heartbeat.h"
#include "exports_classes/host_node_name.h"
#include "exports_classes/loader.hpp"
#include "exports_classes/message.h"
#include "exports_classes/out.h"
#include "exports_classes/parameter.h"
#include "exports_classes/reader.hpp"
#include "exports_classes/receiver.hpp"
#include "exports_classes/schema.h"
#include "exports_classes/sender.hpp"
#include "exports_classes/serial.h"
#include "exports_classes/storage_path.h"
#include "exports_classes/stream.h"
#include "exports_classes/string.hpp"
#include "exports_classes/datetime.h"
#include "exports_classes/timer.h"
#include "exports_classes/udp_socket_receiver.h"
#include "exports_classes/udp_socket_sender.h"


/** Вспомогательный макрос делающий из передаваемой переменной, указетель на нее, размер переменной, и конвертирует имя переменной в строку */
#define IPC_ID(_id)    &_id, sizeof(_id), #_id

#define IPC_ENUM_VALUE(_id) ipc::Schema::Enum::ItemName(#_id, _id)

#define IPC_BOOL(_id)   ipc::Schema::Boolean(&_id, sizeof(_id), #_id )
#define IPC_INT(_id)    ipc::Schema::Number(&_id, sizeof(_id), #_id )
#define IPC_REAL(_id)   ipc::Schema::Number(&_id, sizeof(_id), #_id )
#define IPC_STRING(_id) ipc::Schema::String(&_id, sizeof(_id), #_id )
#define IPC_STRUCT(_id) ipc::Schema::Struct(&_id, #_id, _id.schema())
#define IPC_ARRAY(_id)  ipc::Schema::Array(&_id, sizeof(_id), #_id )
#define IPC_ENUM(_id)   ipc::Schema::Enum(ipc::Schema::Number((int*)&_id, sizeof(_id), #_id))

#define IPC_ONEOF_ENUM(_id, _value) ipc::Schema(this)\
                                    .title(id_schema(IPC_ENUM(_id)).get_item_title(_value))\
                                    .add(IPC_ENUM(_id).add(IPC_ENUM_VALUE(_value)).default_(_value))

#define IPC_BOOLS(_id)  ipc::Schema::Array_boolean(&_id[0], sizeof(_id[0]), #_id, sizeof(_id))
#define IPC_INTS(_id)   ipc::Schema::Array_number(&_id[0], sizeof(_id[0]), #_id, sizeof(_id))
#define IPC_REALS(_id)  ipc::Schema::Array_number(&_id[0], sizeof(_id[0]), #_id, sizeof(_id))
#define IPC_STRINGS(_id)ipc::Schema::Array_string(&_id[0], sizeof(_id[0]), #_id, sizeof(_id))
#define IPC_STRUCTS(_id)ipc::Schema::Array_struct(&_id[0], #_id, _id[0].schema(), sizeof(_id))
#define IPC_ENUMS(_id)  ipc::Schema::Array_enum(ipc::Schema::Number((int*)&_id[0], sizeof(_id[0]), #_id), sizeof(_id))
