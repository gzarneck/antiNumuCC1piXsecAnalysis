# echo "Setting antiNumuCC1piXsecAnalysis v0r0 in /mnt/home/gzarnecki/Highland2/v2r29/highland2"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /mnt/home/gzarnecki/T2K/CMT/v1r20p20081118
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=antiNumuCC1piXsecAnalysis -version=v0r0 -path=/mnt/home/gzarnecki/Highland2/v2r29/highland2  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

