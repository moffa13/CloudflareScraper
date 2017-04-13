# CloudflareScraper

A Qt Cloudflare scraper. Use it if your application needs to bypass cloudflare anti-bot security.

***

## Installation

if you're going to use it as an external library, just run

	qmake src\CloudflareScraper.pro
  
if you want to use the example, run

	qmake "DEFINES+=NO_DLL" src\CloudflareScraper.pro
    
then run

	nmake
 
 or
 
 	make
	

## CLI

To write the html page into a file, simply do

	CloudflareScraper http://www.******.com/ > result.html 2> error.log