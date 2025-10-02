use std::io::ErrorKind;
use std::fs;
use serde::Serialize;

use crate::MizuSubcommands;
use crate::project::project::{self, FileStat};

// Init: initialize project in cwd or with a custom name in the same directory
#[derive(Serialize)]
pub struct SubcommandInit {
    name: Option<String>,
    proj_type: String,
    license: Option<String>
}

impl MizuSubcommands for SubcommandInit {
    fn run(&self) {
        let final_project_name = || {
            if !self.name.is_empty() {
                self.name
            }
            self.name = self.get_probable_projname();
            final_project_name();
        }

        let mut mizurc_needs_changing: bool;

        match self.ensure_sanity() {
            Ok(FileStat::Directory) => std::process::exit(-1),
            Ok(FileStat::Exists) => {
                
            }
        }
    }
}

impl SubcommandInit {
    pub fn initialize_implementation(name: String, proj_type: String, license: String) -> SubcommandInit {
        SubcommandInit {
            name: Some(name),
            proj_type,
            license: Some(license),
        }
    }

    // this time check if a valid mizurc.toml exists
    pub fn ensure_sanity(&self) -> Result<FileStat, std::io::Error> {
        log::info!("Checking if mizurc.toml exists...");

        let res = match fs::metadata("mizurc.toml") {
            Ok(meta) => {
                // not a directory (valid)
                if !meta.is_dir() {
                    // empty (valid)
                    if meta.len() == 0 {
                        log::warn!("mizurc.toml exists but it's empty!");
                        Ok(FileStat::Empty)
                    } else {
                        // file exists as a normal occupied file (valid)
                        Ok(FileStat::Exists)
                    }
                } else {
                    // is a directory (invalid)
                    log::error!("mizurc.toml is in an invalid format!");
                    Ok(FileStat::Directory)
                }
            }

            Err(e) => match e.kind() {
                ErrorKind::NotFound => Ok(FileStat::NonExistent),
                _ => {
                    log::error!("{}", e);
                    Err(e)
                }
            },
        };

        log::info!("All checks passed!");
        res
    }

    // return basename of cwd
    pub fn get_probable_projname(&self) -> String {
        let cwd = project::getcwd().expect("Failed to get CWD");
        project::basename(cwd).expect("Failed to get basename of CWD")
    }
}