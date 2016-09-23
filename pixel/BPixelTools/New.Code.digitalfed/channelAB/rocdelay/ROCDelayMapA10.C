{ // ROOT scripts always need set of {} to do all the script


	//create the stuff needed for plotting
	TCanvas *c1 = new TCanvas ("Histos", "Histos",50,50,800,600);
	c1->cd();



	string fileName = "./rocdelay_30-08-2016_17-24-17";
	
	
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
	
	float b6;
	float b7;
	float delayport0;
	float delayport1;
	float errorRate;
	char  channel;
	string channelstr = "";

	tree->SetBranchAddress("b6",&b6);
	tree->SetBranchAddress("b7",&b7);
	tree->SetBranchAddress("delayport0",&delayport0);
	tree->SetBranchAddress("delayport1",&delayport1);
	tree->SetBranchAddress("errorRate",&errorRate);
	tree->SetBranchAddress("channel",&channel);
	
	int entries= tree->GetEntries();
	
	TH2F *g = new TH2F("map", "map", 8, -0.5, 7.5, 8, -0.5, 7.5);
	for (int i=0; i<entries; i++)
	{
		tree->GetEntry(i);
		channelstr = channel;
		if((channelstr == "A") && (b6 == 1) && (b7 == 0)){
			//cout << "channel: " <<  channel << "pll400: " << pll400delay << ", pll160: " << pll160delay << ", errorRate: " << errorRate << endl;
			g->SetBinContent(delayport0+1, delayport1+1, errorRate);
		}
	}
	
	
	g->GetXaxis()->SetTitle("Delay port 0");
	g->GetYaxis()->SetTitle("Delay port 1");
	g->GetYaxis()->SetTitleOffset(1.2);
	//g->SetMarkerStyle(4);
	//g->SetMarkerColor(kGreen);
	g->SetTitle("ErrorRateMap channel A b6_1 b7_0");
	g->SetStats(false);
	
	g->Draw("colz");
	
	string plotFileName = + fileName + "_ErrorRateMap_channel_A_b6_1_b7_0.pdf";
	c1->SaveAs(plotFileName.c_str());
	plotFileName = fileName + "_ErrorRateMap_channel_A_b6_1_b7_0.png";
	c1->SaveAs(plotFileName.c_str());	


	
	
	

    //f->Close();
	
	
	
	
	
} // end of {} for ROOT
