
#include "sourceGenFunctions.h"

	
/* Next TODOs: 
* - header defines -> DONE
* - Ebenen zählen (+ Funktion fürs Schließen) -> DONE 
* - Template aus config -> DONE 
* - Einzelne Processing Styles -> DONE 
* - Mapping für PS -> DONE
* - Dateinamen bestimmen  -> DONE
* - Automatisch über alle files iterieren -> DONE
* - Convenience function -> DONE
* - Header in Implementierung -> DONE
* - Template- und Funktionsparameter statt std-Wert in convenience function übergeben (vor existierendem Split: nach "=" suchen und splitten) -> DONE
* - Include in Implementierung auf ISA anpassen -> DONE
* - Bitbreite als Section in Config -> DONE
* - Erstellen von implementation files auslagern -> DONE
* - ISA aus config file lesen -> DONE
* - Include file automatisch erstellen 
*/

	int main(void)
	{
  
      //Create the output folders
      mkdir("generated", 0777);
      mkdir("generated/primitives", 0777);
      
      //Initialize configurations
	    cfg_t *primitives_config = cfg_init(primitives_opts, CFGF_NONE);
      cfg_t *class_config = cfg_init(class_opts, CFGF_NONE);  
      
      //Parse global configuration file (configs/config.conf)
      std::cerr << "Try to read ISAs and class names from configs/config.conf\n";
  	    if(cfg_parse(class_config, "configs/config.conf") == CFG_PARSE_ERROR){
  	        std::cerr << "could not open config file\n";
            return 1;
            }
        else
          std::cerr << "Success! Found " << cfg_size(class_config, "classes") << " class name(s) and " << cfg_size(class_config, "isa") << " ISA(s).\n";
     
      
      //We have to store the names of ISAs and classes somewhere
      std::vector<char*> config_files;
      std::vector<char*> isa_names;
      std::vector<char*> class_names;
      std::vector<std::string> f;
      for (int i=0; i< cfg_size(class_config, "isa"); i++) isa_names.push_back(cfg_getnstr(class_config, "isa", i));

      //Which ISAs are in the config file?      
      for (int i=0; i < isa_names.size(); i++){

            config_files.push_back(isa_names[i]);
            std::cerr << "isa " << i << ": " << config_files[i] << "\n";
            mkdir((std::string("generated/")+config_files[i]).c_str(), 0777);
      }
            
      //Which primitives classes are in the config file?      
      for (int i=0; i<cfg_size(class_config, "classes");i++){
        class_names.push_back(cfg_getnstr(class_config, "classes", i));
        std::cerr << "class: " << class_names[i]<< std::endl;  
        for (int j=0; j< isa_names.size();j++)  mkdir((std::string("generated/")+std::string(config_files[j]) + "/primitives").c_str(), 0777);
      } 
      
      
      //Loop over all primitive classes
      for (int m =0; m < class_names.size();m++){
        
        //Initialize the config for the values parsed from configs/<primitive_class>/primitives.conf (inludes the names of all primitives of a class)
        primitives_config = cfg_init(primitives_opts, CFGF_NONE);
        
        std::cerr << "Try to read primitive names from configs/" <<  class_names[m] << "/primitives.conf\n";
        
        //Parse configs/<primitive_class>/primitives.conf 
  	    if(cfg_parse(primitives_config, (std::string(std::string("configs/") + class_names[m] + std::string("/") + std::string("primitives.conf"))).c_str()) == CFG_PARSE_ERROR){
  	        std::cerr << "could not open config file\n";
            return 1;
            }
        else
          std::cerr << "Success! Found " << cfg_size(primitives_config, "primitives") << " primitive names\n";
        
        //Store the names of all primitives of the current class
        std::vector<std::string> primitives;
        for (int k=0; k<cfg_size(primitives_config, "primitives"); k++){
          primitives.push_back(std::string(cfg_getnstr(primitives_config, "primitives", k)));
        }
        // Create the interface file for the current primitive class
        if (createInterface(class_names[m], primitives)==0){
          //Create the implementation files of the current class (the specialization for the processing styles)
          createImplementation(primitives_config, isa_names, class_names, m);
          }else{
          std::cerr << "Error during interface creation.\n";
          }
                        
      }
	    return 0;
	}
