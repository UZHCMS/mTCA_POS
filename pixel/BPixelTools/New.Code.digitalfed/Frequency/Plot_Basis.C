{ // ROOT scripts always need set of {} to do all the script





// Create a Graph with Errors. Read it from the file.

TGraphErrors * my_graph = new TGraphErrors("./Frequency_11-08-2016_18:03:41.txt");
// Now we have the data in my_graph
// by default the first column is the x-Value, the second the y-Value, then the x-Error and then the y-Error.
//plot it:
my_graph->Draw();
my_graph->SetName("Frequency");
my_graph->SetTitle("Frequency");
// If now you run this ROOT script, it should give you a nice map.

} // end of {} for ROOT
