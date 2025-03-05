import os
import shutil

# Source folder
source_folder = r"C:\Dropbox\HUJI\2024\סמסטר א\C and CPP\תרגילים"

# Temporary destination folder to collect filtered files
temp_folder = r"C:\Temp\FilteredProjects"

# Create temporary folder if it doesn't exist
os.makedirs(temp_folder, exist_ok=True)

# Function to copy only files from the top level of "project" subfolders
def copy_project_folders(source, destination):
    for item in os.listdir(source):
        item_path = os.path.join(source, item)
        if os.path.isdir(item_path) and item.lower().startswith("project"):
            # Create a new folder in the temp location
            temp_subfolder = os.path.join(destination, item)
            os.makedirs(temp_subfolder, exist_ok=True)

            # Copy only the files from the top level
            for file in os.listdir(item_path):
                file_path = os.path.join(item_path, file)
                if os.path.isfile(file_path):
                    shutil.copy(file_path, temp_subfolder)

# Execute the function
copy_project_folders(source_folder, temp_folder)

print("done")