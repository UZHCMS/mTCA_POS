void PulseHeight(string file = ""){ // ROOT scripts always need set of {} to do all the script
	
	

	//create the stuff needed for plotting
  string fileName = "";
  if(file != "")
    fileName = file;
  else 
    fileName = "./calinject_08-09-2016_11:33:08";
	
	
	//first create a ROOT file. It automatically becomes the directory to save in.
	string rootFileName = fileName + ".root";
	//TFile f(rootFileName.c_str());
	        
	TFile *f = new TFile(rootFileName.c_str(),"READ");
    //TFile *f = new TFile("/Users/Administrador/Desktop/XTB01-trees/UNIRR_120V.root","RECREATE");
    //ntuple = new TNtuple("ntuple","data from root file","event:time:hit:a:m:c:to:b:chi:invalid");
    if (!f->IsOpen())
        return 1;
         
    cout << "Opened file: " << rootFileName << endl;
        
    f->ls();

  
    tree = (TTree*)f->Get("my_tree");
    tree->Print();
	
	int RocNr;
	int ColAddr;
	int RowAddr;
	int PulseHeight;

	tree->SetBranchAddress("RocNr",&RocNr);            // read in from the tree (generated in a different file: CreateNtupleBenno.C)
	tree->SetBranchAddress("ColAddr",&ColAddr);
	tree->SetBranchAddress("RowAddr",&RowAddr);
	tree->SetBranchAddress("PulseHeight",&PulseHeight);

	TCut cut = "RocNr == 2";


	tree->Draw("PulseHeight>>g(256, -0.5, 255)", cut, "");
	TH1F *g = (TH1F*)gPad->GetPrimitive("g");
	g->GetXaxis()->SetTitle("PulseHeight [ADC]");	

	string plotFileName = + fileName + "_PulseHeight.pdf";
	c1->SaveAs(plotFileName.c_str());
	plotFileName = fileName + "_PulseHeight.png";
	c1->SaveAs(plotFileName.c_str());

}

string IntToString (int number)
{
     stringstream ss;
     ss << number;
     return ss.str();
}
