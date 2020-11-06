# TVLLibGenerator
Welcome to the code generator for the Template Vector Library (TVL).

**Important** At the moment, only the primitive interfaces and sources are built with this generator, not the type definitions or convenience functions. You can find a complete TVL [here](https://github.com/MorphStore/TVLLib). The sources generated by this tool are meant to be equivalent to those in vector/primitives (interfaces) and vector/simd resp. vector/scalar (specializations), excluding extension_\<isa\>.h. We are working on generating the whole TVL with this tool. 

## Build
Call ./build.sh to build the TVL generator.

Building requires g++ and the [libconfuse library](http://www.nongnu.org/confuse/).

## Run
Run ./TVLGenerator. It generates the interface and specialization files for all specified primitive classes, primitives and processing styles.
After running the binary, the generated sources can be found in the folder *generated*.

## Configure
There are different property files, which define the behavior of the generator:

- *configs/config.conf* This is the global property file. It defines the primitive classes and ISAs, for which the sources should be generated.
- *configs/\<primitive_class\>/primitives.conf* For each primitive class, this file lists the primitives for which the sources should be generated.
- *configs/\<primitive_class\>/\<primitive_name\>/interface.conf* Contains the properties required for the interface of a primitive
- *configs/\<primitive_class\>/\<primitive_name\>/\<isa\>.conf* Contains the properties required for the specialization of a primitive for a certain ISA (Instruction Set Architecture). Different register sizes and base types can be supported. This will result in different processing styles for the same ISA. 
  
Note that only the classes, primitives, and ISAs listed in the property files will be used for code generation, even if more folders or property files are present. This enables building only a subset of the TVL.
Feel free to extend the given property files with your own primitives.
  

