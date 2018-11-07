# sdkmesh-to-obj
An open source sdkmesh to obj converter supports multiple meshes conversion

Visual Studio version: Vision Studio 2015 Update3

please refer my repo if used, thanks! https://github.com/StanwieCB/sdkmesh-to-obj

## Standards

### Sdkmesh
The .sdkmesh format is a binary format standard provided by MicroSoft exclusively for DirectX >= 9.0

Sdkmesh uses header + buffer structure. The headers contains detailed declaration for GPU device and the buffers contains continuesly binary vertex buffers and index buffers

Headers include:

- Sdkmesh header
- Sdkmesh vertex buffer headers
- Sdkmesh index buffer headers
- Sdkmesh meshes
- Sdkmesh subsets
- Sdkmesh frames
- Sdkmesh materials

More details can be found in https://github.com/walbourn/contentexporter/wiki/SDKMESH

#### Vertex Structure for Sdkmesh

There are two typical vertex structures provided in this project `PosNormTexTan` and `PosNormTexTan_9`

To determine the actual data structure (like how many bits) used for vertex elements, postion and normal e.g, you must decode the vertex buffer header's declaration first and refer to [D3DDECLTYPE](https://docs.microsoft.com/en-us/windows/desktop/direct3d9/d3ddecltype).

### Obj
The .obj format is a general text-encoded model format

More details can be found in https://en.wikipedia.org/wiki/Wavefront_.obj_file#Material_template_library

### Mtl
The .mtl format is a byproduct format for .obj material discription.

## Installation
Open sdkmesh-to-obj.sIn, build and run it. (my Visual Studio version: Vision Studio 2015 Update3)

## Usage
Change your vertex element structure for vertex buffer vector first or implement your own vertex element structure for loading and writing! (Current supported `PosNormTexTan` and `PosNormTexTan_9`)

> sdkmesh-to-obj.exe -i input_file_path -o output_file_path

example:

> sdkmesh-to-obj.exe -i Squidroom.sdkmesh -o S_room.obj

## Limitations

- Limited implementation for vertex structure
- currently no support for animation

Yiheng Zhang

Visual Computing Enabling Group

Intel Asia-Pacific R&D