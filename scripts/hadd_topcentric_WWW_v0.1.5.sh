if [ -z $1 ]; then
  echo "Must provide the directory path to perfomr the hadding"
  exit
fi

ditto_hadd \
  -1 'www_2l' '-!' www \
  -2 'vh_nonbb' '-@' vh \
  -y 'ttbar_1l,ttbar_dilep,ttg_,tth_,ttw_incl,ttz_incl,tzq_,stt_,sttw_' -Y 'tt-tX-ttX' \
  -o 'zz_2l,zz_2q,zz_4l' -O 'ZZ' \
  -r 'wz_1l,wz_3l,wz_ln' -R 'WZ' \
  -b 'ww_2l2nu,ww_lnuqq,wpwp' -B 'WW' \
  -g 'dy_,wjets_' -G 'V' \
  -d ${1}/haddoutput -i ${1}/output/

#	  -2 'www_2l' '-@' wwwmia \
#	  -3 'vh_nonbb,www_2l' '-#' wwwboth \
