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
In the first Directory (HaBCustomImages) Create a folder Called images and copy all the PAA image files into it

### Step 3
And in the root of the Folder create a file called config.cpp with the following code replace HaBCustomImages with what ever you called the folder in Step 1
```javascript
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

![Part 2 Step 3 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep7.png)

### Step 4
In your Second Directory (HaBCustomImagesPublished) Create 2 folders one called Addons and second called Keys

### Step 5
Create a file called meta.cpp put the following code in there replacing the name with what ever you called your first directory
```javascript
protocol = 1;
publishedid = ;
name = "HaBCustomImages";
timestamp = 0;
```

So your Second Directory (HaBCustomImagesPublished) will look like this
![Part 2 Step 5 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep9.png)

## Part 3 Packing and Signing the Mod

### Step 1
Open Dayz Tools and go to the Addon Builder
Choose the First Directory(HaBCustomImages) as your Source Directory and the Addon Folder in the Second Directory (HaBCustomImagesPublished) as your Destination Directory 
Then Click "Pack"
![Part 3 Step 1 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep10.png)
If it was successfull you should see Build Successfull

### Step 2
Open Dayz Tools and go to DS Utlis
Click "add source directory" and choose the Addon folder in side the Second Diretory(HaBCustomImagesPublished)

### Step 3
Create your Key Click the "N" Beside Private Key to create your Private Key (Unless you already have one)
Give your Private key a name and then click Create Key
![Part 3 Step 3 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep11.png]
Make sure to make a note of the Private Keys Path you will need that later

### Step 4
Click Proccess files and if successfull it should say true under signed next to the PBO
![Part 3 Step 4 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep12.png)

### Step 4 A
If you plan on making the mod public (Allow other servers to use your Icons) you will need to copy youre .bikey from Step 3 into the Keys folder in the Second Directory (DO **NOT** COPY THE .biprivatekey)
![Part 3 Step 4b Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep12c.png)

### Step 4 B
If you don't do Step 4 A you will need to copy your .bikey from Step 3 into the Keys folder on your server in your Dayz Server installation

## Part 4 Publishing your mod to the Steam Workshop

### Step 1
Open up the Dayz Tools and open Publisher Make (New Workshop Item) is selected and then Give your mod a name and choose the Second Directory (HaBCustomImagesPublished) as the Mod Content, You should see Mod Content Structure seems to be valid and Signatures: all signed Check Box I agree and click publish
![Part 4 Step 1 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep14.png)

### Step 2
Get your Mod's Workshop ID open up the mod and copy the id=
![Part 4 Step 2 Image](https://yourenotready.ca/wp-content/uploads/2020/06/IconStep15.png)

### Step 2 A
If you want to allow others to use your mod go back to your Second Directory and edit the meta.cpp to inlcude the ID that you got from step 2 and republish the mod with the Dayz Publisher (Step 1 But Select the mod on the right hand side)

### Step 2 B
If you are just planning on having the mod for your server only you can just edit your meta.cpp then copy the mod to your server manually

## Part 5 Updateing the Levels in the config
### Step 1
Get the file diretory which should be HaBCustomImages\images\BanditCustom.paa for this example replacing HaBCustomImages to what ever the name of your PBO is, and then replaceing BanditCustom to what ever you called your image file
then put that Directory in to the "LevelImage": setting for the level you wish to have the custom images for. 

