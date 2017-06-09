if [ -z $1 ]; then
  echo "Must provide the output directory where the cutflow histogram exists"
  exit
fi
ditto_plot -v SR_counts  -i $1/haddoutput/ -e '--print_cutflow --print_cutflow_bins 1,2,3,4,5,6' -d cutflows_$1/

cat cutflows_$1/SMWWWAnalysis_SR_counts_counter.csv
echo ""
cat cutflows_$1/SMWWWAnalysis_SR_counts_rawcounter.csv

#echo \#\# $(date) > cutflow_temp.md
#cat cutflows_$1/SMWWWAnalysis_SR_counts_counter.csv >> cutflow_temp.md
#echo "" >> cutflow_temp.md
#cat cutflows_$1/SMWWWAnalysis_SR_counts_rawcounter.csv >> cutflow_temp.md

#cat cutflow_temp.md | cat - cutflow.md > temp && mv temp cutflow.md

#	ditto_plot -v SR_counts  -i $1/sync_haddoutput/ -e '--print_cutflow --print_cutflow_bins 1,2,3,4,5,6' -d cutflows_sync_hadd_$1/
#	cat cutflows_sync_hadd_$1/SMWWWAnalysis_SR_counts_*counter.csv
