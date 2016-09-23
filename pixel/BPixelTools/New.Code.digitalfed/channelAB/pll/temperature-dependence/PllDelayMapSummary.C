void PllDelayMapSummary(string fileName = ""){ // ROOT scripts always need set of {} to do all the script
	
	TH2F *g = new TH2F("map", "map", 8, -0.5, 7.5, 8, -0.5, 7.5);
	

	for(int k = 0; k < 216; k++){	

	stringstream ss;
	ss << k;
	fileName = ss.str();
	//create the stuff needed for plotting



	//string fileName = "./pll_newFW_ChannelAB_2-09-2016_11:34:08";
	
	
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
	
	
	int pll400delay;
	int pll160delay;
	float errorRate;
	char  channel;
	string channelstr = "";

	tree->SetBranchAddress("pll400delay",&pll400delay);
	tree->SetBranchAddress("pll160delay",&pll160delay);
	tree->SetBranchAddress("errorRate",&errorRate);
	tree->SetBranchAddress("channel",&channel);
	
	int entries= tree->GetEntries();
	
	for (int i=0; i<entries; i++)
	{
		tree->GetEntry(i);
		channelstr = channel;
		if(channelstr == "G"){
			cout << "channel: " <<  channel << ", pll400: " << pll400delay << ", pll160: " << pll160delay << ", errorRate: " << errorRate << endl;
		if(errorRate != 0){
			//double val = g->GetBinContent(pll400delay+1, pll160delay+1);	
			g->Fill(pll400delay, pll160delay);
			}
		}
	}

	}

	TCanvas *c1 = new TCanvas ("Histos", "Histos",50,50,800,600);
	c1->cd();
	



	g->GetXaxis()->SetTitle("160 MHz PLL Delay");
	g->GetYaxis()->SetTitle("400 MHz PLL Delay");
	g->GetYaxis()->SetTitleOffset(1.2);
	//g->SetMarkerStyle(4);
	//g->SetMarkerColor(kGreen);
	g->SetTitle(ss.str().c_str());
	g->SetStats(false);
	
	g->Draw("colz");
	
	string plotFileName = "ErrorRateSummary.pdf";
	c1->SaveAs(plotFileName.c_str());
	plotFileName = "ErrorRateSummary.png";
	c1->SaveAs(plotFileName.c_str());	

} // end of {} for ROOT
