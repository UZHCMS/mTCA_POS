{ // ROOT scripts always need set of {} to do all the script
	
	string fileName = "./rocdelay_06-09-2016_12:05:08";
	
	
	//first create a ROOT file. It automatically becomes the directory to save in.
	string rootFileName = fileName + ".root";
	TFile f(rootFileName.c_str(), "RECREATE");
	
	f.cd();
	
	
	//create a ROOT tree, automatically filled from the ascii file.
	TTree * t = new TTree();
	t.SetName("my_tree");
	t.SetTitle("my_tree");
	string branchDescriptor = "b7/F:b6/F:delayport0/F:delayport1/F:channel/C:frequency/F:goodword/F:toterror/f:errorRate/F:badevents/F";
	string dataFileName = fileName + ".txt";
	t.ReadFile(dataFileName.c_str(), branchDescriptor.c_str());
	
	//check what is in the tree...
	t.Print();
	
	
	//write the tree to the file, write the file, and close it
	t.Write();
	f.Write();
	f.Close();
} // end of {} for ROOT
