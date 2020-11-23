# TVLLibGenerator
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

- *configs/config.conf* This is the global property file. It defines the primitive classes and ISAs, for which the sources should be generated.
- *configs/\<primitive_class\>/primitives.conf* For each primitive class, this file lists the primitives for which the sources should be generated.
- *configs/\<primitive_class\>/\<primitive_name\>/interface.conf* Contains the properties required for the interface of a primitive
- *configs/\<primitive_class\>/\<primitive_name\>/\<isa\>.conf* Contains the properties required for the specialization of a primitive for a certain ISA (Instruction Set Architecture). Different register sizes and base types can be supported. This will result in different processing styles for the same ISA. 
  
Note that only the classes, primitives, and ISAs listed in the property files will be used for code generation, even if more folders or property files are present. This enables building only a subset of the TVL.
Feel free to extend the given property files with your own primitives.

## Supported Instruction Sets
For supporting an instruction set, not only the implementation of the primitives is necessary, but also a mapping of the instruction specific data types, i.e. masks and vector registers. At the moment, this mapping exists for the following instruction sets:
- All: Scalar
- Intel: SSE, AVX2, AVX512
- ARM: NEON, SVE (currently not supported by the generator, but you can find a hard-coded prototype backend in our [TVL repository](https://github.com/MorphStore/TVLLib/tree/simd-arm_sve/vector/simd/sve)
- NEC: Tsubasa SX Aurora
All mappings can be found in *header/extension_\<isa\>.h*.
You are, however, welcome to contribute the mappings for other instruction sets.

## Using the TVL
Include the files *"header/vector_extension_structs.h"* and *"generated/\<isa\>/primitives/\<primitiv_class\>_\<isa\>.h"* and you are ready to use each included primitive, e.g. the *add* primitive for AVX512 and 64 bit data elements (from generated/avx512/primitives/calc.h): 

```c++
using processingStyle = avx512<v512<uint64_t>>;

//This brings some convenient access to constants, e.g. vector_base_t_granularity::value 
IMPORT_VECTOR_BOILER_PLATE(processingStyle) 

//Load something into two vector registers vector1 and vector2 of type vector_t.
//You may use the load primitive for this purpose.

//Call the add primitive
vector_t resultVec = add<processingStyle, vector_base_t_granularity::value>(vector1, vector2);
```

**Note** This generator harmonizes the interfaces of all primitives. Each primitive is a struct with a name suffix, e.g. *add_t*, which has an apply function. Additionally, there is a convenience function, e.g. *add(..)*, which calls the apply function. Thus, if you are coming from an older TVL version and decided to use the generated code, you might have to refactor you existing code to the partially new interface, i.e. delete any direct calls of the apply function, or add the *_t*-suffix.

### Processing Style and additional template parameters
A processing style is a nested template parameter, which defines the instruction set, the vector size, and the type of the data elements. In the code above, the instruction set is AVX512, the vector registers are 512 bit wide (note that AVX512 can also deal with smaller registers), and the base data is made of unsigned 64-bit integers.

You can use different processig styles with different instruction sets, even in the same compilation unit. But be aware that the convenient access of the boiler plate constants only work for one processing style at a time. You can, however (a) access them directly but with ugly syntax, e.g. *processingStyle::vector_helper_t::granularity::value* or (b) use a prefix when importing the boiler plate and using the constants (see header/vector_extension_structs.h l.45-54).

A processing style is always the first template parameter required by a primitive. Other template parameters may be defined if necessary. Use the *templates*-property in the *interface.conf* files to define additional template parameters and optional default values. The number and value of these additional template parameters, which are regarded in the implementation are defined by the *nr_additional_template_parameters*- and *template_parameters*-properties in the *\<isa\>.conf* files.
In the example above, there is one additional template parameter.

### Build
Compile with the according flags required for all instruction sets, you are using and include one or more additional flags (DSSE, DAVXTWO, DAVX512, or DNEON) to make the TVL include only the necessary headers.
For instance, the code above can be compiled using *g++ -DAVXTWO -DAVX512 -mavx2 -mavx512f mySouceFile.cpp*.

#### Known issues
- Depending on the instruction set you are using, not all parameters are requied for each implementation. To disable the according warning when compiling using g++, you may use the flag *-Wno-unused-parameter*.
