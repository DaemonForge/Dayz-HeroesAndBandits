class HaBJSONHandler<Class T>
{
	protected static ref JsonSerializer m_Serializer = new JsonSerializer;
	
	static void FromString(string stringData, out T data)
	{
		string error;
		if (!m_Serializer)
			m_Serializer = new JsonSerializer;
		
		if (!m_Serializer.ReadFromString(data, stringData, error)) {
			habPrint("Error Creating Data from Json", "Exception");
		}
	}
	
	static string ToString(T data)
	{
		string stringData;
		if (!m_Serializer)
			m_Serializer = new JsonSerializer;

		if (!m_Serializer.WriteToString(data, true, stringData)) {
			habPrint("Error Creating JSON from Data", "Exception");
			return "";
		}
		
		return stringData;
	}

	static void FromFile(string path, out T data)
	{
		if (!FileExist(path)) {
			habPrint("File At" + path + " could not be found", "Exception");
			return;
		}

		FileHandle fh = OpenFile(path, FileMode.READ);
		string jsonData;
		string error;

		if (fh) {
			
			string line;
			while (FGets(fh, line) > 0) {
				jsonData = jsonData + "\n" + line;
			}

			CloseFile(fh);
			
			bool success = m_Serializer.ReadFromString(data, jsonData, error);
			
			if (error != "" || !success) {
				habPrint("File At" + path + " could not be parsed", "Exception");
			} else {
				habPrint("File Opened from" + path + "", "Debug");
			}
		} else {
			habPrint("File At" + path + " could not be opened", "Exception");
		}
	}
	
	static void ToFile(string path, T data)
	{
		FileHandle fh = OpenFile(path, FileMode.WRITE);
			
		if (!fh) {
			habPrint("File At" + path + " could not be created", "Exception");
			return;
		} 
		
		string jsonData;
		bool success = m_Serializer.WriteToString(data, true, jsonData);

		if (success && jsonData != string.Empty) {
			FPrintln(fh, jsonData);
		}

		habPrint("File Saved At" + path + "", "Debug");
		CloseFile(fh);
	}

};