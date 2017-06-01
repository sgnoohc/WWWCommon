#!/bin/bash

if [ -z $1 ]; then
  echo "Error: must provide a tag name!"
  echo " $0 TAG [extra option to ditto_runana]"
  exit
fi

TAG=$1
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
EXTRAOPTION=$2

ditto_runana -d /nfs-7/userdata/bhashemi/WWW_babies/WWW_v0.1.5/skim/ -l ${DIR}/../inputlist/WWW_v0.1.5_inputlist.txt -t WWW_v0.1.5_${TAG} -f ${EXTRAOPTION}
#	&& hadd_WWW_v0.1.5.sh WWW_v0.1.5_${TAG}
