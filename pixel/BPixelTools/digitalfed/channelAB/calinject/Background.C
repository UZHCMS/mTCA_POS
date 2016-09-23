//void PllDelayMapSummary(string fileName = "")            // function to read in the fileName when calling 'root' (root ("actualFileName"), or so)
void Background(string file = ""){ // ROOT scripts always need set of {} to do all the script
	
  TH2F *g = new TH2F("map", "map", 416, 0.5, 416.5, 162, 0.5, 162.5);     //create a histogram
	

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


	int entries= tree->GetEntries();
	
	for (int i=0; i<entries; i++)
	{
		tree->GetEntry(i);
		if( (RocNr == 1) || (RocNr == 2) || (RocNr == 3) || (RocNr == 4) || (RocNr == 5) || (RocNr == 6) || (RocNr == 7) || (RocNr == 8) ){
		        cout << "RocNr: " <<  RocNr << ", ColAddr: " << ColAddr << ", RowAddr: " << RowAddr  << ", PulseHeight: " << PulseHeight << endl;   
		        g->Fill( (52-ColAddr+1) + (RocNr-1)*52, (81-RowAddr+1) );}                        // fills the histogram roc 0-7
		else if ( (RocNr == 9) || (RocNr == 10) || (RocNr == 11) || (RocNr == 12) || (RocNr == 13) || (RocNr == 14) || (RocNr == 15) || (RocNr == 16) ){
		        cout << "RocNr: " <<  RocNr << ", ColAddr: " << ColAddr << ", RowAddr: " << RowAddr  << ", PulseHeight: " << PulseHeight << endl;
		        g->Fill( ((ColAddr+1) + (16-RocNr)*52) , ( RowAddr+1+81) );}                                                                                      // fills the histogram roc 8-15

	}


	TCanvas *c1 = new TCanvas ("Histos", "Histos",50,50,800,600);           // create a "windows" for the histogram
	c1->cd();
	



	g->GetXaxis()->SetTitle("Col");
	g->GetYaxis()->SetTitle("Row");
	g->GetYaxis()->SetTitleOffset(1.2);
	//g->SetMarkerStyle(4);
	//g->SetMarkerColor(kGreen);
	g->SetTitle("Hits per Pixel");
	g->SetStats(false);
	
	g->Draw("colz");

	string plotFileName = + fileName + "_HitMap.pdf";
	c1->SaveAs(plotFileName.c_str());
	plotFileName = fileName + "_HitMap.png";
	c1->SaveAs(plotFileName.c_str());

/*	string cut = "";
	for(int roc = 1; roc <= 16; roc++){
				cout << "\t" << roc << " / 16" << endl;
		for(int col = 1; col <= 52; col++){
			for(int row = 1; row <= 81; row++){
				if( (RocNr == 1) || (RocNr == 2) || (RocNr == 3) || (RocNr == 4) || (RocNr == 5) || (RocNr == 6) || (RocNr == 7) || (RocNr == 8) ){
					if(g->GetBinContent((ColAddr+1) + (RocNr-1)*52, (RowAddr+1)) > 1){
						cut = cut + "((RocNr==" + IntToString(roc) + ")&&(ColAddr==" + IntToString(col) + ")&&(RowAddr==" + IntToString(row) + ")) ||";}
				} else if ( (RocNr == 9) || (RocNr == 10) || (RocNr == 11) || (RocNr == 12) || (RocNr == 13) || (RocNr == 14) || (RocNr == 15) || (RocNr == 16) ){
					if(g->GetBinContent(((ColAddr+1) + (16-RocNr)*52) , ( RowAddr+1+81) ) > 1){
						cut = cut + "((RocNr==" + IntToString(roc) + ")&&(ColAddr==" + IntToString(col) + ")&&(RowAddr==" + IntToString(row) + ")) ||";}
				}
			}
		}		
	}
	cout << "Cut for all pixels with less than two hits: " << endl;
	cout << cut << endl;
*/
}

string IntToString (int number)
{
     stringstream ss;
     ss << number;
     return ss.str();
}
