pub mod project {
	use std::env;
	use std::fs;
	use std::path::{Path, PathBuf};
	use serde::{Deserialize, Serialize};
	use toml::{Table, Value};

	#[derive(Serialize)]
	pub struct Config<T> {
		pub mizu_config: T,
	}

	#[derive(Deserialize)]
	pub struct ConfigForValidation<T> {
		title: String,
		pub rest_of_config: T,
	}

	pub enum FileStat {
		Exists,
		Directory,
		NonExistent,
		Empty
	}

	pub enum ConfigValidity {
		Valid,
		Invalid
	}

	// A wrapper for fs::create_dir()
	pub fn mkdir(dir: String) -> i8 {
		log::info!("About to create directory '{}'", dir);

		match fs::create_dir(dir) {
			Ok(_) => {
				log::info!("Succesfully created directory");
				0
			}
			Err(e) => {
				log::error!("Failed to create directory!");
				log::error!("Error:	{}", e);
				-1
			}
		}
	}

	// A wrapper for env::current_dir()
	pub fn getcwd() -> Result<PathBuf, ()> {
		log::info!("About to get current working directory");

		match env::current_dir() {
			Ok(path) => {
				log::info!("Succesfully got path: {:?}", path);
				Ok(path)
			}
			Err(e) => {
				log::error!("Failed to get path: {}", e);
				Err(())
			}
		}
 	}

 	// A wrapper for `Path.file_name()` (basename equivalent in rust)
 	pub fn basename(path: PathBuf) -> Result<String, ()> {
 		if let Some(path_osstr) = path.file_name() {
 			if let Some(retval) = path_osstr.to_str() {
 				Ok(retval.to_string())
 			} else {
                Err(())
            }
 		} else {
 			Err(())
 		}
 	}

	pub fn get_dir_create_filepath(dir: String) -> String {
		let cwd = getcwd();
		let filepath =  cwd.expect("Cannot join path").join(dir);
		filepath.to_string_lossy().to_string()
	}

	pub fn create_config_file<T: Serialize>(name: &str, config_data: &T) -> Result<String, std::io::Error> {
		let config_opts = Config {
			mizu_config: config_data,
		};

		log::info!("Writing configuration file for project '{}'", name);
		let config = toml::to_string(&config_opts).unwrap();

		// join both project directory and config file
		let binding = get_dir_create_filepath(name.to_string());
		let project_path = Path::new(&binding);
		let conf_file_name = "mizurc.toml";
		let conf_file_path = project_path.join(conf_file_name);

		// now write config
		match std::fs::write(&conf_file_path, config) {
			Ok(_) => {
				log::info!("Succesfully wrote configuration file");
				Ok(conf_file_path.to_string_lossy().to_string())
			}
			Err(e) => {
				log::error!("Failed to write configuration file");
				Err(e)
			}
		}
	}

	pub fn validate_config_file<T: Serialize>(config_file: String /*, config_data: &T */) -> Result<ConfigValidity, ()> {
		let config_file_contents = |file: String| {
			fs::read_to_string(file);
		};

		let values: ConfigForValidation<T> = toml::from_str(config_file_contents(config_file)).expect("Cannot parse config file");

		for key in &values {
			if key == "mizu_config" {
				return Ok(ConfigValidity::Valid);
			}
		}

		Err(())
	}
}