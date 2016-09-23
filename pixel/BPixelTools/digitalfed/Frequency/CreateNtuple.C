{ // ROOT scripts always need set of {} to do all the script
	
	string fileName = "./Frequency_11-08-2016_18-03-41";
	
	
	//first create a ROOT file. It automatically becomes the directory to save in.
	string rootFileName = fileName + ".root";
	TFile f(rootFileName.c_str(), "RECREATE");
	
	
	//create a ROOT tree, automatically filled from the ascii file.
	//TTree * t = new TTree();
	TNtuple * t = new TTree();
	string branchDescriptor = "freq/F:correctEvents/F:errorRate/F:errorUncert/F:badEvents/F:errorCount/F:missingHeaders/F:missingTrailers/F:flippedHT/F:eventNrErrors/F:tbmMissedTriggers/F:unknownErrors/F:eventNrResets/F";
	string dataFileName = fileName + ".txt";
	t.ReadFile(dataFileName.c_str(), branchDescriptor.c_str());
	
	//check what is in the tree...
	t.Print();
	
	
	//write the tree to the file and close it
	f.Write();
	f.Close();
} // end of {} for ROOT