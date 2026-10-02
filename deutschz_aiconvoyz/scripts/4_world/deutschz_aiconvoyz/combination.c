class deutschz_recipe_create_toxicz_zone_marker: RecipeBase
{
    override void Init()
    {
        m_Name = "ToxicZ-Signalgeraet entschluesseln";
        m_IsInstaRecipe = false; m_AnimationLength = 2; m_Specialty = 0.02;
        InsertIngredient(0, "Toxicz_Doc_Decoder");
        InsertIngredient(1, "ToxicZ_Secret_Document");
        m_IngredientAddHealth[0] = 0; m_IngredientAddHealth[1] = 0;
        m_IngredientSetHealth[0] = -1; m_IngredientSetHealth[1] = -1;
        m_IngredientAddQuantity[0] = 0; m_IngredientAddQuantity[1] = 0;
        m_IngredientDestroy[0] = true; m_IngredientDestroy[1] = true;
        m_ResultSetFullQuantity[0] = false; m_ResultSetQuantity[0] = -1;
        m_ResultSetHealth[0] = -1; m_ResultInheritsHealth[0] = -1;
        m_ResultToInventory[0] = -2; m_ResultUseSoftSkills[0] = false;
        m_ResultReplacesIngredient[0] = -1;
        AddResult("ToxicZ_Signal_Marker");
    }

    override bool CanDo(ItemBase ingredients[], PlayerBase player) { return player && ingredients[0] && ingredients[1]; }
    override void Do(ItemBase ingredients[], PlayerBase player, array<ItemBase> results, float specialty_weight)
    {
        if (!GetGame() || !GetGame().IsServer() || !player || !player.GetIdentity()) return;
        string dir = "$profile:DeutschZ-System/deutschz_radiomissionz/event_completions/combined";
        MakeDirectory("$profile:DeutschZ-System"); MakeDirectory("$profile:DeutschZ-System/deutschz_radiomissionz"); MakeDirectory("$profile:DeutschZ-System/deutschz_radiomissionz/event_completions"); MakeDirectory(dir);
        FileHandle file = OpenFile(dir + "/" + player.GetIdentity().GetPlainId() + ".done", FileMode.WRITE); if (file != 0) { FPrintln(file, "combined"); CloseFile(file); }
    }
}

modded class PluginRecipesManagerBase
{
    override void RegisterRecipies()
    {
        super.RegisterRecipies();
        RegisterRecipe(new deutschz_recipe_create_toxicz_zone_marker());
    }
}
