modded class ActionBandageBase extends ActionContinuousBase
{
	override void ApplyBandage( ItemBase item, PlayerBase player )
	{	
		super.ApplyBandage( item, player );
		PlayerBase sourcePlayer = PlayerBase.Cast(player);
		if (sourcePlayer.GetBleedingManagerServer().GetBleedingSourcesCount() == 0){
			sourcePlayer.habResetBleedingSource();
		}
	}
	
};
