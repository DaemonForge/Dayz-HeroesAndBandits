# How to create your own Custom Images

This is how you can create your own mod to allow you to use your own Custom Icons with the Heroes and Bandit Mod

## Part 1 Creating the Images

### Step 1 Scale Image
Using youre favorite editing tool make sure the image is at the resloution of either 512x512, 256x256 or 128x128

![Step 1 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep1.png)

### Step 2 Export the Image
Using that same tool export the image it is important that it has a transparnt background

![Step 2 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep2.png)

### Step 3 Convert the Image to PAA
Open Dayz Tools and go to the Image To PAA Tool add the directory where you saved the image and click Proccess Files

![Step 3 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep6.png)

If successfull you will see green Text saying Converted Successfully

## Part 2 Create Mod Folder Stuctures

### Step 1
Create two Folders with the name of your mod in my example I am calling them HaBCustomImages and second HaBCustomImagesPublished

### Step 2
In the first Directory(HaBCustomImages) Create a folder Called images and copy all the PAA image files you made into there
And in the root of the Folder create a file called config.cpp with the following code replace HaBCustomImages with what ever you called the folder in Step 1
```
class CfgPatches
{
	class HaBCustomImages
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={
		};
	};
};

class CfgMods
{
	class HaBCustomImages
	{
		dir="HaBCustomImages";
        picture="";
        action="";
        hideName=1;
        hidePicture=1;
        name="HaBCustomImages";
        credits="DaemonForge";
        author="DaemonForge";
        authorID="0";
        version="0.1";
        extra=0;
        type="mod";
	    dependencies[]={ };
	    class defs
	    {
			
        };
    };
};
```
So your first folder should look like this

![Part 2 Step 2 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep7.png)

### Step 3
In your second directory (HaBCustomImagesPublished) Create 2 folders one called Addons and second called Keys

### Step 4
Create a file called meta.cpp put the following code in there replacing the name with what ever you called your first directory
```
protocol = 1;
publishedid = ;
name = "HaBCustomImages";
timestamp = 0;
```

So your second directory will look like this
![Part 2 Step 4 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep9.png)


