# Firstly Set-ExecutionPolicy UnRestricted -Force

Invoke-WebRequest -Uri https://github.com/curl/curl/releases/download/curl-7_55_1/curl-7.55.1.tar.gz -OutFile curl-7.55.1.tar.gz
Invoke-WebRequest -Uri https://github.com/GNOME/libxml2/archive/v2.9.9.tar.gz -OutFile libxml2-2.9.9.tar.gz
Invoke-WebRequest -Uri https://github.com/open-source-parsers/jsoncpp/archive/1.8.4.tar.gz -OutFile jsoncpp_1.8.4.tar.gz
# curl.exe -L url --ssl-no-revoke --output outputname

# Finally Set-ExecutionPolicy Restricted -Force