if [ -z $1 ]; then
  echo "Must provide the output directory where the cutflow histogram exists"
  exit
fi
ditto_plot -v SR_counts  -i $1/haddoutput/ -e '--print_cutflow --print_cutflow_bins 1,2,3' -d plots_$1/
cat plots_$1/SMWWWAnalysis_SR_counts_counter.csv
echo ""
cat plots_$1/SMWWWAnalysis_SR_counts_rawcounter.csv

#	ditto_plot -v SR_counts  -i $1/sync_haddoutput/ -e '--print_cutflow --print_cutflow_bins 1,2,3,4,5,6' -d plots_sync_hadd_$1/
#	cat plots_sync_hadd_$1/SMWWWAnalysis_SR_counts_*counter.csv
