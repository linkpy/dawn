//* Copyright 2019 The Dawn Authors
//*
//* Licensed under the Apache License, Version 2.0 (the "License");
//* you may not use this file except in compliance with the License.
//* You may obtain a copy of the License at
//*
//*     http://www.apache.org/licenses/LICENSE-2.0
//*
//* Unless required by applicable law or agreed to in writing, software
//* distributed under the License is distributed on an "AS IS" BASIS,
//* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//* See the License for the specific language governing permissions and
//* limitations under the License.

#ifndef DAWNWIRE_SERVER_SERVERBASE_H_
#define DAWNWIRE_SERVER_SERVERBASE_H_

#include "dawn/dawn_proc_table.h"
#include "dawn/wire/ChunkedCommandHandler.h"
#include "dawn/wire/Wire.h"
#include "dawn/wire/WireCmd_autogen.h"
#include "dawn/wire/WireDeserializeAllocator.h"
#include "dawn/wire/server/ObjectStorage.h"

namespace dawn::wire::server {

    class ServerBase : public ChunkedCommandHandler, public ObjectIdResolver {
      public:
        ServerBase() = default;
        ~ServerBase() override = default;

      protected:
        void DestroyAllObjects(const DawnProcTable& procs) {
            //* Release devices first to force completion of any async work.
            {
                std::vector<WGPUDevice> handles = mKnownDevice.AcquireAllHandles();
                for (WGPUDevice handle : handles) {
                    procs.deviceRelease(handle);
                }
            }
            //* Free all objects when the server is destroyed
            {% for type in by_category["object"] if type.name.get() != "device" %}
                {
                    std::vector<{{as_cType(type.name)}}> handles = mKnown{{type.name.CamelCase()}}.AcquireAllHandles();
                    for ({{as_cType(type.name)}} handle : handles) {
                        procs.{{as_varName(type.name, Name("release"))}}(handle);
                    }
                }
            {% endfor %}
        }

        {% for type in by_category["object"] %}
            const KnownObjects<{{as_cType(type.name)}}>& {{type.name.CamelCase()}}Objects() const {
                return mKnown{{type.name.CamelCase()}};
            }
            KnownObjects<{{as_cType(type.name)}}>& {{type.name.CamelCase()}}Objects() {
                return mKnown{{type.name.CamelCase()}};
            }
        {% endfor %}

        {% for type in by_category["object"] if type.name.CamelCase() in server_reverse_lookup_objects %}
            const ObjectIdLookupTable<{{as_cType(type.name)}}>& {{type.name.CamelCase()}}ObjectIdTable() const {
                return m{{type.name.CamelCase()}}IdTable;
            }
            ObjectIdLookupTable<{{as_cType(type.name)}}>& {{type.name.CamelCase()}}ObjectIdTable() {
                return m{{type.name.CamelCase()}}IdTable;
            }
        {% endfor %}

      private:
        // Implementation of the ObjectIdResolver interface
        {% for type in by_category["object"] %}
            WireResult GetFromId(ObjectId id, {{as_cType(type.name)}}* out) const final {
                return mKnown{{type.name.CamelCase()}}.GetNativeHandle(id, out);
            }

            WireResult GetOptionalFromId(ObjectId id, {{as_cType(type.name)}}* out) const final {
                if (id == 0) {
                    *out = nullptr;
                    return WireResult::Success;
                }

                return GetFromId(id, out);
            }
        {% endfor %}

        //* The list of known IDs for each object type.
        {% for type in by_category["object"] %}
            KnownObjects<{{as_cType(type.name)}}> mKnown{{type.name.CamelCase()}};
        {% endfor %}

        {% for type in by_category["object"] if type.name.CamelCase() in server_reverse_lookup_objects %}
            ObjectIdLookupTable<{{as_cType(type.name)}}> m{{type.name.CamelCase()}}IdTable;
        {% endfor %}
    };

}  // namespace dawn::wire::server

#endif  // DAWNWIRE_SERVER_SERVERBASE_H_
