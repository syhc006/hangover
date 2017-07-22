/*
 * Copyright 2017 Stefan Dösinger for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/* The basic idea behind declaring these structures outside dlls/d3d9/ is that
 * other DLLs like d3dx9 will need them for functions that take an IDirect3D9*
 * interface somewhere. They'll need to wrap the d3d9 stuff in addition to
 * their own interfaces. */

#ifndef QEMU_D3D9_WRAPPER_H
#define QEMU_D3D9_WRAPPER_H

/* These structures are accessed by both guest and host, including the guest
 * application code. This may need extra care for 32<->64 mapping because
 * we can't just make everything an uint64_t. */

struct qemu_d3d9_impl
{
    IDirect3D9Ex IDirect3D9Ex_iface;
    IDirect3D9Ex *host;
};

struct qemu_d3d9_device_impl
{
    IDirect3DDevice9Ex IDirect3DDevice9Ex_iface;
    IDirect3DDevice9Ex *host;
    struct qemu_d3d9_impl *d3d9;
};

struct qemu_d3d9_subresource_impl
{
    union
    {
        IDirect3DSurface9 IDirect3DSurface9_iface;
        IDirect3DVolume9 IDirect3DVolume9_iface;
    };
    union
    {
        IDirect3DSurface9 *host;
        IDirect3DVolume9 *host_volume;
    };
};

struct qemu_d3d9_swapchain_impl
{
    IDirect3DSwapChain9Ex IDirect3DSwapChain9Ex_iface;
    IDirect3DSwapChain9Ex *host;

    IUnknown private_data;
    ULONG private_data_ref; /* NOT the externally visible ref! */
    struct qemu_d3d9_device_impl *device;
    struct qemu_d3d9_subresource_impl backbuffers[1];
};

struct qemu_d3d9_texture_impl
{
    IDirect3DBaseTexture9 IDirect3DBaseTexture9_iface;
    IDirect3DBaseTexture9 *host;

    IUnknown private_data;
    ULONG private_data_ref; /* NOT the externally visible ref! */
    struct qemu_d3d9_device_impl *device;

    struct qemu_d3d9_subresource_impl subs[1];
};

static inline struct qemu_d3d9_texture_impl *impl_from_IDirect3DTexture9(IDirect3DTexture9 *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d9_texture_impl, IDirect3DBaseTexture9_iface);
}

static inline struct qemu_d3d9_texture_impl *impl_from_IDirect3DCubeTexture9(IDirect3DCubeTexture9 *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d9_texture_impl, IDirect3DBaseTexture9_iface);
}

static inline struct qemu_d3d9_texture_impl *impl_from_IDirect3DVolumeTexture9(IDirect3DVolumeTexture9 *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d9_texture_impl, IDirect3DBaseTexture9_iface);
}

struct qemu_d3d9_vertex_declaration_impl
{
    IDirect3DVertexDeclaration9 IDirect3DVertexDeclaration9_iface;
    IDirect3DVertexDeclaration9 *host;

    struct qemu_d3d9_device_impl *device;
};

static inline struct qemu_d3d9_vertex_declaration_impl *impl_from_IDirect3DVertexDeclaration9(IDirect3DVertexDeclaration9 *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d9_vertex_declaration_impl, IDirect3DVertexDeclaration9_iface);
}

#endif
