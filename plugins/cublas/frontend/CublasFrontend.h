/*
 * gVirtuS -- A GPGPU transparent virtualization component.
 *
 * Copyright (C) 2009-2010  The University of Napoli Parthenope at Naples.
 *
 * This file is part of gVirtuS.
 *
 * gVirtuS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * gVirtuS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gVirtuS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Written by: Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>,
 *             Department of Applied Science
 */
#ifndef CUBLASFRONTEND_H
#define	CUBLASFRONTEND_H

// Deprecated to include both in 2.1.15.cuBLAS: Release 12.0
// #include <cublas.h>
// #include <cublas_api.h>
#include "cublas_v2.h"
#include <cuda_runtime_api.h>

#include <gvirtus/frontend/Frontend.h>
//#include "Cublas.h"

class CublasFrontend {
public:
    static inline void Execute(const char *routine, const gvirtus::communicators::Buffer *input_buffer = NULL) {
        gvirtus::frontend::Frontend::GetFrontend()->Execute(routine, input_buffer);
    }

    /**
     * Prepares the Frontend for the execution. This method _must_ be called
     * before any requests of execution or any method for adding parameters for
     * the next execution.
     */
    static inline void Prepare() {
        gvirtus::frontend::Frontend::GetFrontend()->Prepare();
    }

    static inline gvirtus::communicators::Buffer *GetLaunchBuffer() {
        return gvirtus::frontend::Frontend::GetFrontend()->GetInputBuffer();
    }

    /**
     * Adds a scalar variabile as an input parameter for the next execution
     * request.
     *
     * @param var the variable to add as a parameter.
     */
    template <class T> static inline void AddVariableForArguments(T var) {
      gvirtus::frontend::Frontend::GetFrontend()->GetInputBuffer()->Add(var);
    }

    /**
     * Adds a string (array of char(s)) as an input parameter for the next
     * execution request.
     *
     * @param s the string to add as a parameter.
     */
    static inline void AddStringForArguments(const char *s) {
      gvirtus::frontend::Frontend::GetFrontend()->GetInputBuffer()->AddString(s);
    }

    /**
     * Adds, marshalling it, an host pointer as an input parameter for the next
     * execution request.
     * The optional parameter n is usefull when adding an array: with n is
     * possible to specify the length of the array in terms of elements.
     *
     * @param ptr the pointer to add as a parameter.
     * @param n the length of the array, if ptr is an array.
     */
    template <class T>static inline void AddHostPointerForArguments(T *ptr, size_t n = 1) {
      gvirtus::frontend::Frontend::GetFrontend()->GetInputBuffer()->Add(ptr, n);
    }

    /**
     * Adds a device pointer as an input parameter for the next execution
     * request.
     *
     * @param ptr the pointer to add as a parameter.
     */
    static inline void AddDevicePointerForArguments(const void *ptr) {
      gvirtus::frontend::Frontend::GetFrontend()->GetInputBuffer()->Add((uint64_t) ptr);
    }

    /**
     * Adds a symbol, a named variable, as an input parameter for the next
     * execution request.
     *
     * @param symbol the symbol to add as a parameter.
     */
    static inline void AddSymbolForArguments(const char *symbol) {
        /* TODO: implement AddSymbolForArguments
         * AddStringForArguments(CudaUtil::MarshalHostPointer((void *) symbol));
         * AddStringForArguments(symbol);
         * */
    }

    static inline cublasStatus_t GetExitCode() {
        return (cublasStatus_t) gvirtus::frontend::Frontend::GetFrontend()->GetExitCode();
    }

    static inline bool Success() {
        return gvirtus::frontend::Frontend::GetFrontend()->Success(cudaSuccess);
    }

    template <class T> static inline T GetOutputVariable() {
        return gvirtus::frontend::Frontend::GetFrontend()->GetOutputBuffer()->Get<T> ();
    }

    /**
     * Retrives an host pointer from the output parameters of the last execution
     * request.
     * The optional parameter n is usefull when retriving an array: with n is
     * possible to specify the length of the array in terms of elements.
     *
     * @param n the length of the array.
     *
     * @return the pointer from the output parameters.
     */
    template <class T>static inline T * GetOutputHostPointer(size_t n = 1) {
        return gvirtus::frontend::Frontend::GetFrontend()->GetOutputBuffer()->Assign<T> (n);
    }

    /**
     * Retrives a device pointer from the output parameters of the last
     * execution request.
     *
     * @return the pointer to the device memory.
     */
    static inline void * GetOutputDevicePointer() {
        return (void *) gvirtus::frontend::Frontend::GetFrontend()->GetOutputBuffer()->Get<uint64_t>();
    }

    /**
     * Retrives a string, array of chars, from the output parameters of the last
     * execution request.
     *
     * @return the string from the output parameters.
     */
    static inline char * GetOutputString() {
        return gvirtus::frontend::Frontend::GetFrontend()->GetOutputBuffer()->AssignString();
    }
    CublasFrontend();
    static void * handler;
};
#endif	/* CUBLASFRONTEND_H */


