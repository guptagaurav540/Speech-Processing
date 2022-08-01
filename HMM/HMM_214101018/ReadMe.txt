========================================================================
    CONSOLE APPLICATION : HMM_214101018 Project Overview
========================================================================

AppWizard has created this HMM_214101018 application for you.

This file contains a summary of what you will find in each of the files that
make up your HMM_214101018 application.


HMM_214101018.cpp
    This is the main application source file. and HMM functions 


LBG.h
	this is header file which contain the funcions of genrating code book, Universe,Normalization, DC shift 


roll_number[1024]="214101018"				This is the file starting name of the speech utternces   (214101018_0_1.txt)				(roll_number_digit_utterncenumber.txt)
folder_path[1024]="214101018_digit/";		This is the folder name where the speech utternces are stored You can change if want to train the module with others sound 

IF you want to change the sound files change the roll_number and folder_path variable 

How to run -
	Press f5 key

if you want to genrate the codebook and universe file i have commennted the option 4 and 5 please uncomment and run 
	case 4: will genrate the universe 
	case 5: will gwnrate the codebook of size 32 from the universe 

The given module have 91% accuracy with my sound and live testing is also working good.
Accuracy of live testing is 75-80%
