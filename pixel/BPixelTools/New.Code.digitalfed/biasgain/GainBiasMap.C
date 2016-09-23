{ // ROOT scripts always need set of {} to do all the script


	//create the stuff needed for plotting
	TCanvas *c1 = new TCanvas ("Histos", "Histos",50,50,800,600);
	c1->cd();



	string fileName = "./biasgain_22-08-2016_17-37-21";
	
	
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
	
	
	float gain;
	float bias;
	float errorRate;
	char  channel;
	tree->SetBranchAddress("gain",&gain);
	tree->SetBranchAddress("bias",&bias);
	tree->SetBranchAddress("errorRate",&errorRate);
	tree->SetBranchAddress("channel",&channel);
	
	int entries= tree->GetEntries();
	
	TH2F *g = new TH2F("map", "map", 4, -0.5, 3.5, 128, -0.5, 127.5);
	for (int i=0; i<entries; i++)
	{
		tree->GetEntry(i);
		cout << "channel:" << channel << ", gain: " << gain << ", bias: " << bias << ", errorRate: " << errorRate << endl;
		if(channel == "G")
			g->SetBinContent(gain+1, bias+1, errorRate);
	}
	
	
	g->GetXaxis()->SetTitle("POH Laser Gain [DAC]");
	g->GetYaxis()->SetTitle("POH Laser Bias [DAC]");
	g->GetYaxis()->SetTitleOffset(1.2);
	//g->SetMarkerStyle(4);
	//g->SetMarkerColor(kGreen);
	g->SetTitle("");
	g->SetStats(false);
	
	g->Draw("colz");
	
	string plotFileName = + fileName + "_ErrorRateMap.pdf";
	c1->SaveAs(plotFileName.c_str());
	plotFileName = fileName + "_ErrorRateMap.png";
	c1->SaveAs(plotFileName.c_str());	


	
	
	

    //f->Close();
	
	
	
	
	
} // end of {} for ROOT
