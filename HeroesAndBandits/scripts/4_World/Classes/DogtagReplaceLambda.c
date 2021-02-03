#ifdef WRDG_DOGTAGS
modded class DogtagReplaceLambda : ReplaceItemWithNewLambdaBase
{

	override void OnCopyProperties(Dogtag_Base old_tag, Dogtag_Base new_tag)
	{
		super.OnCopyProperties( old_tag,  new_tag);
		new_tag.SetHaBHumanity(old_tag.GetHaBHumanity());
		new_tag.SetHaBAffinity(old_tag.GetHaBAffinity());
	}
}
#endif