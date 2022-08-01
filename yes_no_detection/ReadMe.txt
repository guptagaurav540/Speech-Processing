Instruction to execute-
	Directly Build or run
	
	if you want to change input file 
	file_name variable store the name of input file.

Input file-
	yes_no.txt => containing the four consecutive yes and four no.
	DC.txt => contain the system error wave form which is used in dc-shift.

steps-

	1. DC shift - I am calculating DC shift.
		for calculating DC shift we take a input file DC.txt.

	2. Normalization - For calculating normalization i take the mod of maximum amplitude. Normalized data store in norm.txt

		normalization factor=(5000/maximum amplitude);

	3. Skip first 1000 samples so that starting error will remove 

	4. storing energy and zcr value per window in enzcr.txt file by taking a window size 320;

	5. calculating ambient noise by averaging first 5 winodow energy.
	
		Threshold energy= (ambient noise energy)*10

	6. finding start and end point by analysing energy.
		when consecutive three window have energy greater then the threshold energy then sound starts.
		when consecutive three window have energy less then the threshold energy then sound end.

	7. Calculating ZCR values. ZCR1 is average of 70% of sound and ZCR2 is average of last 30% sound.
		if(ZCR2>ZCR1) then sound is "YES" otherwise "NO".

Working of code- 
	
	dc_shift()=> Function used for calculate the dc shift.
		
		dc_dhift(DC.txt)=> return the dc shift 

	normalization()=> Function used for normalizing the wave form.
		 normalization function take yes_no.txt as input and produce norm.txt file as output.
		 for normalization we find the maximum amplitude and scale it in range of -5000 to 5000. 

	after that skip 1000 sample from norm.txt and calculate energy and zcr from norm.txt by taking a window and put the values in enzcr.txt 
	- window size 320

	- After that I store calculate the energy and zcr value per window and store it in enzcr.txt.

	-Finding ambient noise.
		We take average energy of 5 window from starting for finding ambient energy.
		and after that we calculate the threshold energy.
			threshold energy=ambient energy*5;
	
	- Sound finding from enzcr.txt file 

		- we check the energy for finding the sounding 
		- if three consecutive window have energy greater then the ambient noise then sound starts and if the three consecutive window have 
		energy less then the ambient noise then the sound end.
		
		- if <zcr> of last 30% is greater then starting 70% part then sound is "Yes".
		otherwise sound is "No".