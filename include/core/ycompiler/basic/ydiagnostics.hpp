/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
 * @Author: Sky
 * @Date: 2021-11-20 09:22:37
 * @LastEditors: Sky
 * @LastEditTime: 2021-11-27 13:50:25
 * @Description: 
 * @FilePath: /yLib/include/core/ycompiler/basic/ydiagnostics.hpp
 * https://github.com/flyinskyin2013/yLib
 */


#ifndef __CORE_YCOMPILER_BASIC_YDIAGNOSTICS_HPP__
#define __CORE_YCOMPILER_BASIC_YDIAGNOSTICS_HPP__

#include "core/yobject.hpp"
#include "core/ycompiler/basic/ysource_location.hpp"
#include "core/ycompiler/lexer/ytoken.hpp"
#include "core/ycompiler/basic/ysource_manager.hpp"
#include <memory>
#include <limits>

#include <cassert>

namespace yLib
{
    namespace ycompiler
    {
        class yDiagnosticsEngine;
        class yCompilerInstance;

        namespace diag{

            enum DiagID{
                #define DIAG_INFO(diag_name, tag_name, level, info, argnum) \
                    diag_name,

                #include "ydiagnostic_kinds.def"
                NUM_DIAGID
                #undef DIAG_INFO
            };
        }

        // Options for controlling the compiler diagnostics engine.
        class __YLIB_CLASS_DECLSPEC__ yDiagnosticOptions:
        YLIB_PUBLIC_INHERIT_YOBJECT
        {
            public:
            yDiagnosticOptions();
            ~yDiagnosticOptions();
        };

        // Used for handling and querying diagnostic IDs.
        class __YLIB_CLASS_DECLSPEC__ yDiagnosticsIDHandle:
        YLIB_PUBLIC_INHERIT_YOBJECT
        {
            public:
            yDiagnosticsIDHandle();
            ~yDiagnosticsIDHandle();

            std::string get_description(diag::DiagID id);
        };

        //formats and prints fully processed diagnostics.
        class __YLIB_CLASS_DECLSPEC__ yDiagnosticConsumer:
        YLIB_PUBLIC_INHERIT_YOBJECT
        {
            public:
            yDiagnosticConsumer();
            ~yDiagnosticConsumer();            
        };


        //===----------------------------------------------------------------------===//
        // DiagnosticBuilder
        //===----------------------------------------------------------------------===//

        /// A little helper class used to produce diagnostics.
        ///
        /// This is constructed by the DiagnosticsEngine::Report method, and
        /// allows insertion of extra information (arguments and source ranges) into
        /// the currently "in flight" diagnostic.  When the temporary for the builder
        /// is destroyed, the diagnostic is issued.
        ///
        /// Note that many of these will be created as temporary objects (many call
        /// sites), so we want them to be small and we never want their address taken.
        /// This ensures that compilers with somewhat reasonable optimizers will promote
        /// the common fields to registers, eliminating increments of the NumArgs field,
        /// for example.
        class yDiagnosticBuilder{
            private:
            yDiagnosticsEngine * diag_engine = nullptr;

            protected:
            /// Force the diagnostic builder to emit the diagnostic now.
            ///
            /// Once this function has been called, the DiagnosticBuilder object
            /// should not be used again before it is destroyed.
            ///
            /// \returns true if a diagnostic was emitted, false if the
            /// diagnostic was suppressed.
            bool emit(){

                return true;
            }

            public:
            ~yDiagnosticBuilder(){emit();}
            yDiagnosticBuilder(yDiagnosticsEngine * diag_engine):diag_engine(diag_engine){

                assert(diag_engine && "DiagnosticBuilder requires a valid DiagnosticsEngine!");
                // assert(DiagStorage &&
                //     "DiagnosticBuilder requires a valid DiagnosticStorage!");
            }
        };

        /// Concrete class used by the front-end to report problems and issues.
        class __YLIB_CLASS_DECLSPEC__ yDiagnosticsEngine:
        YLIB_PUBLIC_INHERIT_YOBJECT
        {
            private:
            std::unique_ptr<yDiagnosticsIDHandle> diag_id_handle;
            std::unique_ptr<yDiagnosticOptions> diag_options;
            std::unique_ptr<yDiagnosticConsumer> diag_consumer;
            ySourceManager& src_mgr;


            /// The location of the current diagnostic that is in flight.
            ySourceLocation cur_diag_loc;

            /// The ID of the current diagnostic that is in flight.
            ///
            /// This is set to std::numeric_limits<unsigned>::max() when there is no
            /// diagnostic in flight.
            unsigned cur_diag_id;

            public:
            yDiagnosticsEngine() = delete;
            yDiagnosticsEngine(std::unique_ptr<yDiagnosticsIDHandle> && diag_id_handle, 
                                std::unique_ptr<yDiagnosticOptions> && diag_options,
                                std::unique_ptr<yDiagnosticConsumer> && diag_consumer,
                                ySourceManager& src_mgr);

            void DiagReport(ySourceLocation & loc, diag::DiagID id);
            void DiagReport(yToken &token, diag::DiagID id);

            /// Issue the message to the client.
            ///
            /// This actually returns an instance of DiagnosticBuilder which emits the
            /// diagnostics (through @c ProcessDiag) when it is destroyed.
            ///
            /// \param DiagID A member of the @c diag::kind enum.
            /// \param Loc Represents the source location associated with the diagnostic,
            /// which can be an invalid location if no position information is available.
            inline yDiagnosticBuilder Report(ySourceLocation loc,
                                                            unsigned diag_id) {
                assert(cur_diag_id == std::numeric_limits<unsigned>::max() &&
                        "Multiple diagnostics in flight at once!");
                cur_diag_loc = loc;
                cur_diag_id = diag_id;

                return yDiagnosticBuilder(this);
            }
        };



    } // namespace ycompiler
} // namespace yLib

#endif //__CORE_YCOMPILER_BASIC_YDIAGNOSTICS_HPP__