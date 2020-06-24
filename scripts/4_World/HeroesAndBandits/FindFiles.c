TStringArray HABFindFilesInDirectory(string directory) 
{ 
	
	TStringArray fileList = new TStringArray;
	
	string      fileName;
    int     	fileAttr;
    int         flags;
	
    string pathpattern = directory + "\\*";
	
	FindFileHandle fileHandler = FindFile(pathpattern, fileName, fileAttr, flags);
	
	bool found = true;
        while ( found )
        {                
	        if ( fileAttr )
	        {
	        	fileList.Insert(fileName);
	        }  
         found = FindNextFile(fileHandler, fileName, fileAttr);
        }

    return fileList; 
}