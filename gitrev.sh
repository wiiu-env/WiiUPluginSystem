#! /bin/bash
#
rev_new=$(git rev-parse --short=7 HEAD)
version=$(cat ./loader/src/version.h 2>/dev/null | cut -d '"' -f2)


rev_date=`date -u +%Y%m%d%H%M%S`

    cat <<EOF > ./loader/meta/meta.xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<app version="1">
  <name>Wii U Plugin Loader</name>
  <coder>Maschell</coder>
  <url>https://github.com/Maschell/WiiUPluginSystem</url>
  <version>$version-nightly-$rev_new</version>
  <release_date>$rev_date</release_date>
  <short_description>Plugin loader for the Wii U</short_description>
  <long_description></long_description>
  <category>tool</category>

</app>
EOF

    cat <<EOF > ./loader/src/version.h
#define APP_VERSION "$version-nightly-$rev_new"
EOF


echo $version-nightly-$rev_new
