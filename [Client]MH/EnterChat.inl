
//////////////////////////////////////////////////////////////////////////
// ÄÉ¸¯ÅÍ¸¦ ¿òÁ÷ÀÎ´Ù

//ÇöÀç ¿©±â·Î ¾Èµé¾î¿Â´Ù.
/*
void EnterChat(CKeyboard& Keyboard,CMouse& Mouse)
{
	// textarea¿Í Ãæµ¹..taiyo 
//	if(WINDOWMGR->IsComposition()) return;
	
//¿¢Æ¼ºê»óÅÂ°¡ ¾Æ´Ï¸é ¸®ÅÏ
//´Ù¸¥ ¿¡µðÆ® Æ÷Ä¿½º ÀÖÀ¸¸é ¸®ÅÏ
//¸ÅÅ©·Î ¸ðµå°¡ Ã¤ÆÃ¿ì¼±ÀÌ¸é ¿£ÅÍÅ°°¡ ¾Æ´Ï¶ó Ã¤ÆÃ Å°·Î È®ÀÎ ¶Ç´Â Ç×»ó Æ÷Ä¿½º?
	if(Keyboard.GetKeyUp(KEY_RETURN))
	{
		cEditBox* pChatDlg = (cEditBox*)WINDOWMGR->GetWindowForIDEx(MI_CHATEDITBOX);
		if(pChatDlg->IsFocus() == FALSE)
			pChatDlg->SetFocusEdit(TRUE);	
		
		else
		{
			if(strlen(pChatDlg->GetEditText()) == 0)
				pChatDlg->SetFocusEdit(FALSE);
			else
			{
				pChatDlg->SetEditText("");
				pChatDlg->SetFocusEdit(TRUE);
			}
		}
	}
}
*/

