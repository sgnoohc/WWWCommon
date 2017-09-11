if [ -z $1 ]; then
  echo "Must provide the output directory where the cutflow histogram exists"
  exit
fi
if [[ $1 == *"0.1.11" ]]; then
    ditto_plot -v SR_  -i $1/haddoutput/ -e '--autostack --print_cutflow --print_cutflow_bins 7,8,9' -d cutflows_$1/
else
    ditto_plot -v SR_  -i $1/haddoutput/ -e '--autostack --print_cutflow --print_cutflow_bins 1,2,3,4,5,6,7,8,9' -d cutflows_$1/
fi

echo ""
cat cutflows_$1/SR_counter.csv
echo ""
cat cutflows_$1/SR_rawcounter.csv

#echo \#\# $(date) > cutflow_temp.md
#cat cutflows_$1/SMWWWAnalysis_SR_counts_counter.csv >> cutflow_temp.md
#echo "" >> cutflow_temp.md
#cat cutflows_$1/SMWWWAnalysis_SR_counts_rawcounter.csv >> cutflow_temp.md

#cat cutflow_temp.md | cat - cutflow.md > temp && mv temp cutflow.md

#	ditto_plot -v SR_counts  -i $1/sync_haddoutput/ -e '--print_cutflow --print_cutflow_bins 1,2,3,4,5,6' -d cutflows_sync_hadd_$1/
#	cat cutflows_sync_hadd_$1/SMWWWAnalysis_SR_counts_*counter.csv
