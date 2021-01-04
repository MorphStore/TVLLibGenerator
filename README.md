

# TVLLibGenerator

[Build](#build)  
[Run](#run)  
[Configure](#configure)  
---[Global Configuration File (config.conf)](#globalconfig)  
---[Class Configuration Files (primitives.conf)](#classconfig)  
---[Primitive Interface Configuration (interface.conf)](#primitiveconfig)  
---[Primitive Backend (\<isa\>.conf)](#backendconfig)  
[Supported Instruction Sets](supported-instruction-sets)  
[Using the TVL](using-the-tvl)  
---[Processing Style and Additional Template Parameters](processing-style-and-additional-template-parameters)  
---[Build and Link](build-and-link)  
------[Building the examples](building-the-examples)  
------[Known issues](known-issues)

Welcome to the code generator for the Template Vector Library (TVL).

**Important** At the moment, only the primitive interfaces and sources are built with this generator. The type definitions and any convenience functions are located in the folder *header*. This should work out of the box now, but it is very early stage, so this is not guaranteed. You can find a complete TVL with more primitives and example code [here](https://github.com/MorphStore/TVLLib). The sources generated by this tool are meant to be equivalent to those in vector/primitives (interfaces) and vector/simd resp. vector/scalar (specializations), excluding extension_\<isa\>.h. 

## Build
Call ./build.sh to build the TVL generator.

Building requires g++ and the [libconfuse library](http://www.nongnu.org/confuse/).

## Run
Run ./TVLGenerator. It generates the interface and specialization files for all specified primitive classes, primitives, and processing styles.
After running the binary, the generated sources can be found in the folder *generated*. Other files required for using the TVL are located in the folder *header*.

## Configure
There are different property files, which define the behavior of the generator:

- **Global Configuration File** *configs/config.conf* This is the global property file. It defines the primitive classes and ISAs, for which the sources should be generated.
- **Class Configuration Files** *configs/\<primitive_class\>/primitives.conf* For each primitive class, this file lists the primitives for which the sources should be generated. If additional headers are required by a primitive class, they are also noted here.
- **Primitive Interface Configuration** *configs/\<primitive_class\>/\<primitive_name\>/interface.conf* Contains the properties required for the interface of a primitive
- **Primitive Backend** *configs/\<primitive_class\>/\<primitive_name\>/\<isa\>.conf* Contains the properties required for the specialization of a primitive for a certain ISA (Instruction Set Architecture). Different register sizes and base types can be supported. This will result in different processing styles for the same ISA. 
  
Note that only the classes, primitives, and ISAs listed in the property files will be used for code generation, even if more folders or property files are present. This enables building only a subset of the TVL.
Feel free to extend the given property files with your own primitives.

In the following, the properties and values for each type of property files is explained.

How to read:

**property** *\[type of value\]* Description

<a id="globalconfig"></a>
### Global Configuration File (config.conf)
- **classes** *\[list of strings\]* Lists all primitive classes going to be generated. Currently, there are 7 classes: io, calc, compare, create, extract, logic, and manipulate. Make sure to match the class name with the name of the primitive class directory. If you introduce another class for whatever reason, make sure to include it manually in *header/vector_primitives*...And let us know of it. We have been fine with the 7 classes, but different applications might require different solutions.
- **isa** *\[list of strings\]* All ISAs, which are supported. Make sure to name the property files, which are specific for a primitive backend, accordingly, e.g. sse.conf for the sse-backend.

<a id="classconfig"></a>
### Class Configuration Files (primitives.conf)
- **primitives** *\[list of strings\]* The name of all primitives of this class. Name the directories with the interfaces and backends accordingly.
- **header** *\[list of strings\]* If this primitive class requires any additional includes, they go here. For relative reference, just provide the relative path. If you want to include a header from a linked library, surround them with *<>* brackets (additonally to the quotation marks). The headers will be included for each ISA. If you require ISA-specific includes, see below.
- **isa_header** *\[list of strings\]* If one or more ISAs require additional include headers, list these ISAs and the include files here: {"<ISA 1>", "<Header 1 for ISA 1>",..."<Header n for ISA 1>", "<ISA 2>",  "<Header 1 for ISA 2>",...}
- **isa_header_idx** *\[list of int\]* This is required if *isa_header* is set, for the following reason: *isa_header* is a list of strings, but the generator needs to know which of these strings is an ISA name and which strings are the names of the header files. Thus, *isa_header_idx* lists the indexes of the ISA names in the list provided by *isa_header*. The index is 0-based!

<a id="primitiveconfig"></a>
### Primitive Interface Configuration (interface.conf)
The interface files are used to create the interface headers and provide a part of the information used for the implementation. To make the implementation work, use *ProcessingStyle* whenever you would referenc ethe processing style in the code.
- **primitive_class** *\[string\]* The name of the primitive class, ideally one of the 7 already supported classes.
- **primitive_name** *\[string\]* The name of the primitive.
- **templates** *\[list of strings\]* If any additional template parameters are used, they go here. If there are default values, you can provide them just like you would do it in the code, e.g. 
```.properties
# Additional templates for the add primitive
templates = {"int Granularity = ProcessingStyle::vector_helper_t::granularity::value"}
```
 The processing style is always generated automatically as the first template parameter. Hence, it is not required to provide it here.
- **return_type** *\[string\]* The return type of the primitive, mostly either void or  depending on the processing style, e.g. typename ProcessingStyle::vector_t.
- **arguments** *\[list of strings\]* All arguments the primitive takes, including default values (if necessary), e.g.
```.properties
# Arguments of the add primitive
arguments = {"typename ProcessingStyle::vector_t const & p_vec1", "typename ProcessingStyle::vector_t const & p_vec2", "int element_count = ProcessingStyle::vector_helper_t::element_count::value"}
```

<a id="backendconfig"></a>
### Primitive Backend (\<isa\>.conf)
The backend properties are provided in sections, where each section contains the properties for a specific vector size.
The following properties are available:
- **base_types** *\[list of strings\]* A list of all base types to be supported.
- **implementations** *\[list of strings\]* The implementations for all base types. This works fine with line breaks in case your implementations are a wee bit longer.
- **nr_additional_template_parameters** *\[unsigned int\]* If there are any template parameter specializations (except for the processing style, which is generated automatically), specify how many there are.
- **template parameters** *\[list of strings\]* All additional template specializations. If there is more than one additional template argument and more than one base type, follow this sequence: {base type 1/argument1, base type 1/argument 2, ..., base type n/argument 1, base type n/argument 2,..., base type n/argument m}  

An example for the add primitive for avx2 (configs/calc/add/avx2.conf) looks like the following:
```.properties
register_size 128
{
  base_types = {"uint64_t", "uint32_t"}
  implementations = {"return _mm_add_epi64( p_vec1, p_vec2);", "return _mm_add_epi32( p_vec1, p_vec2);"} 
  nr_additional_template_parameters = 1
  template_parameters = {"64", "32"}
}

register_size 256
{
  base_types = {"uint64_t", "uint32_t"}
  implementations = {"return _mm256_add_epi64( p_vec1, p_vec2);", "return _mm256_add_epi32( p_vec1, p_vec2);"} 
  nr_additional_template_parameters = 1
  template_parameters = {"64", "32"}
}
```
AVX2 natively supports 256 bit registers, but is also downward compatible to 128 bit registers (which is sometimes the better choice). Thus, we introduced two sections, one for 128 bit and one for 256 bit. For starters, we made it support two differenty sized unsigned integers. The implementations for each of them is a one liner. There is one additional template argument, which we provide for each base type. We could just use the default for this (the ganularity of the base type), but in case we ever want to reinterpret our values, the specialization makes sense. Additonally, it's a great and simple example for demonstration.

## Supported Instruction Sets
For supporting an instruction set, not only the implementation of the primitives is necessary, but also a mapping of the instruction specific data types, i.e. masks and vector registers. At the moment, this mapping exists for the following instruction sets:
- All: Scalar
- Intel: SSE, AVX2, AVX512
- ARM: NEON, SVE (currently not supported by the generator, but you can find a hard-coded prototype backend in our [TVL repository](https://github.com/MorphStore/TVLLib/tree/simd-arm_sve/vector/simd/sve))
- NEC: Tsubasa SX Aurora
- GPU: CUDA (experimental)

All mappings can be found in *header/extension_\<isa\>.h*.
You are, however, welcome to contribute the mappings for other instruction sets.

## Using the TVL
Include the files *"header/vector_extension_structs.h"* and *"generated/\<isa\>/primitives/\<primitive_class\>_\<isa\>.h"* as well as the according primitive interface, and you are ready to use each included primitive, e.g. the *add* primitive for AVX512 and 64 bit data elements (from generated/avx512/primitives/calc.h): 

```c++
#include <header/extension_avx512.h>
#include <header/vector_extension_structs.h>
#include <generated/primitives/calc.h>
#include <generated/avx512/primitives/calc_avx512.h>

using processingStyle = avx512<v512<uint64_t>>;

//This brings some convenient access to constants, e.g. vector_base_t_granularity::value 
IMPORT_VECTOR_BOILER_PLATE(processingStyle) 

//Load something into two vector registers vector1 and vector2 of type vector_t.
//You may use the load primitive for this purpose.

//Call the add primitive
vector_t resultVec = add<processingStyle, vector_base_t_granularity::value>(vector1, vector2);
```
For convenience, there is a header file (*header/vector_primitives.h*), which already includes all interfaces and primitive backends. However, this works only if there is an interface header for each of the 7 primitive classes and a backend for each ISA, which will be used.

**Note** This generator harmonizes the interfaces of all primitives. Each primitive is a struct with a name suffix, e.g. *add_t*, which has an apply function. Additionally, there is a convenience function, e.g. *add(..)*, which calls the apply function. Thus, if you are coming from an older TVL version and decided to use the generated code, you might have to refactor you existing code to the partially new interface, i.e. delete any direct calls of the apply function, or add the *_t*-suffix.

### Processing Style and Additional Template Parameters
A processing style is a nested template parameter, which defines the instruction set, the vector size, and the type of the data elements. In the code above, the instruction set is AVX512, the vector registers are 512 bit wide (note that AVX512 can also deal with smaller registers), and the base data is made of unsigned 64-bit integers.

You can use different processig styles with different instruction sets, even in the same compilation unit. But be aware that the convenient access of the boiler plate constants only work for one processing style at a time. You can, however (a) access them directly but with ugly syntax, e.g. *processingStyle::vector_helper_t::granularity::value* or (b) use a prefix when importing the boiler plate and using the constants (see header/vector_extension_structs.h l.45-54).

A processing style is always the first template parameter required by a primitive. Additional template parameters may be defined if necessary. Use the *templates*-property in the *interface.conf* files to define additional template parameters and optional default values (as described [above](#primitiveconfig)). The number and value of these additional template parameters, which are regarded in the implementation are defined by the *nr_additional_template_parameters*- and *template_parameters*-properties in the *\<isa\>.conf* files.
In the example above, there is one additional template parameter.

### Build and Link
There are 3 things necessary for a successful build process:
1. Compile with the according flags required for all instruction sets, you are using.
2. Include a flag for each instruction set the TVLwill be using (DSCALAR, DSSE, DAVXTWO, DAVX512, DTSUBASA, DCUDA, or DNEON). This ensures that only the necessary headers are included. This sounds redundant to point 1, but it prevents you from getting compile errors if you are crazy enough to mix native vector code with TVL code that works with another ISA.
3. Link the directory, which contains the *header* and *generated* folders.

For instance, the code above can be compiled using

*g++ -DAVXTWO -DAVX512 -mavx2 -mavx512f -I/TVLLibGenerator mySouceFile.cpp*

where *TVLLibGenerator* is the folder containing the *header* and *generated* folders, and mySouceFile.cpp is the source code using the TVL.

#### Building the examples
Examples can be found in the *examples* folder. The script *build_examples.sh* builds all of them and requires an environment variable *TVL*, which contains the path to the TVL, i.e. the folder containing the *header* and *generated* folders. Usually, this is simply the TVLLIBGenerator root folder.
Currently, the examples use AVX512 and the NEC Tsubasa vector engine.

##### Example 1: libtest
This example tests is the TVL is working at all and provides an example for the basic use of TVL primitives. In detail, the following steps are performed:
1. A processing style is set and the according boiler plate is imported for more convenience.
2. The values *1* and *2* are broadcasted into two vector registers. 
3. The two registers are added element-wise.
4. The the elements of the vector resulting from step 3 are added. The result is a scalar value.
5. The vector containg the value *1* is subtracted from the vector result from step 3. Then, the elements of this result are added. The result is a scalar value.
6. Repeat step 5, but do a multiplication instead.
7. Repeat step 5, but do a division instead.
8. The results from steps 4-7 are printed. They depend on the vector size. 

The reslts from step 4,6, and 7 should be the same. The result from step 5 should be reduced by the amount of vector elements of the used backend, e.g. reduced by *1* for a scalar processing style.

Unlike **Example 2**, libtest hardcodes the processing style, i.e. you have to change it during development. However, this is easily done by including the corresponding isa-header file and changing the used processing style. Some examples are already shown as comments in the code.

##### Example 2: Aggregation benchmark
This example performs an aggregation on randomly generated integer data (around 200 MB) and measures the elapsed time (excluding data generation). This example shows how to easily implement a solution for different targets without changing the actual operator code, but only the definition of the processing style. Depending on the compiler flag (see *build\_examples.sh*), the benchmark is built either for AVX512, or for the NEC Tsubasa vector engine.
This examples can also serve as a quick way to get a first impression of the performance of different backends. It can easily be adapted to different backends, because only 3 primitives are used: load (io class), add (calc class), and hadd (calc class). 
Note that the performance may significantly vary depending on the amount of values and the vector register size, especially for small amounts. One reason is that small amounts fit in the cache (the exact amount depends on your system). The other reason is that any rest, which does not fill a vector register anymore, is processed in a scalar way. 

#### Known issues
- Depending on the instruction set you are using, not all parameters are required for each implementation. To disable the according warning when compiling using g++, you may use the flag *-Wno-unused-parameter*.
- Compiler Error *"No such file or directory"*. If this is hrown from *vector\_primitives.h*, a primitive class has either no interface (*generated/primitives/\**), or no backend for the system you are trying to build for (*generated/\<isa\>/primitives/\**).  
  - Quick fix: Comment out the according lines in *vector\_primitives.h*. This will work as long as you are not actually using a non-existent primitive, e.g. when using the provided examples.  
  - Long fix: Implement the according interface and/or backend, e.g. when you introduce a new backend or primitive (class).  
- Compiler error: *reference is ambiguous*. Depending on other libraries you include, some primitive names might already be used for something else. No, the compiler is confused and doesn't know which one to take. Resovle this by providing the namespace, e.g. *vectorlib::div* instead of *div* .
