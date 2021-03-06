void CreateNtupleBenno(string file = ""){ // ROOT scripts always need set of {} to do all the script
  cout << file << endl;
  string fileName = file;//"./calinject_08-09-2016_11:33:08";
	
	
	//first create a ROOT file. It automatically becomes the directory to save in.
	string rootFileName = fileName + ".root";
	TFile f(rootFileName.c_str(), "RECREATE");
	
	f.cd();
	
	
	//create a ROOT tree, automatically filled from the ascii file.
	TTree * t = new TTree();
	t.SetName("my_tree");
	t.SetTitle("my_tree");
	string branchDescriptor = "RocNr/I:ColAddr/I:RowAddr/I:PulseHeight/I";
	string dataFileName = fileName + ".txt";
	t.ReadFile(dataFileName.c_str(), branchDescriptor.c_str());
	
	//check what is in the tree...
	t.Print();
	
	
	//write the tree to the file, write the file, and close it
	t.Write();
	f.Write();
	f.Close();
} // end of {} for ROOT
