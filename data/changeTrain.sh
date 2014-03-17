#!/bin/bash

func()
{
	temp1Count=$(ls temp -l | grep '^-' | wc -l)
	temp2Count=$(ls temp2 -l | grep '^-' | wc -l)

	if [ $temp1Count -gt $temp2Count ]
	then
		mv trainingDocuments/* temp2/
		mv temp/* trainingDocuments/
		echo "store in temp2"
	else
		mv trainingDocuments/* temp/
		mv temp2/* trainingDocuments/
		echo "store in temp"
	fi
}
func
