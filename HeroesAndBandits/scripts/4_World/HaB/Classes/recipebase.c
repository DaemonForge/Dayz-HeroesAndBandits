/**
 * File: MyDayZModClass.c
 * Description: A template class for DayZ modding.
 */


modded class RecipeBase
{
    
 	override bool CheckRecipe(ItemBase item1, ItemBase item2, PlayerBase player)
	{
		return (super.CheckRecipe(item1, item2, player) && player.HaBCanCraft(this));
	}
}
