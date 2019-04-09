<%@ Control Language="c#" AutoEventWireup="false" Codebehind="commendgift.ascx.cs" Inherits="web.control.commendgift" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="528" border="0">
	<TR>
		<TD colspan="3">
			<P><b>推薦新人領獎:</b></P>
			<P>複製下面框框裡的文字，粘貼到各大網站，論壇，MSN，QQ等地方，別人點了含有你ID的註冊地址註冊後，你將會得到獎勵。</P>
			<P>獎勵規則：</P>
			<P>1.當你所推薦的一個玩家到達<span style="FONT-WEIGHT:bold;COLOR:#ff3333">1</span>轉后，你就可以在這裡領取<span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=Application["game.commendmoney"].ToString()%></span>遊戲幣，<span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=Application["game.commendgold"].ToString()%></span>金幣</P>
			<P>2.每次領取後，遊戲幣將存進你的網上銀行</P>
			<P>3.嚴禁惡意推薦自己刷錢，查到後大小號全部刪除</P>
			<P>&nbsp;</P>
		</TD>
	</TR>
	<tr>
		<td style="WIDTH: 30%; HEIGHT: 14px">登陸帳號</td>
		<td style="HEIGHT: 14px"><span style="FONT-WEIGHT:bold;COLOR:#5d51f1"><%=Session["userid"]%></span></td>
		<td style="HEIGHT: 14px"></td>
	</tr>
	<TR>
		<TD style="WIDTH: 233px; HEIGHT: 20px">現有推薦總人數</TD>
		<TD style="WIDTH: 120px; HEIGHT: 20px"><span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=allppl%></span></TD>
		<TD style="HEIGHT: 20px"></TD>
	</TR>
	<TR>
		<TD style="WIDTH: 233px; HEIGHT: 19px">已經一轉人數(可領取數)</TD>
		<TD style="WIDTH: 120px; HEIGHT: 19px"><span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=resetppl%></span></TD>
		<TD style="HEIGHT: 19px"></TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" height="10"></TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" height="10">推薦系統統計結果：本次你可以領取<span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=money%></span>遊戲幣，<span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=gold%></span>金幣</TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" height="10"></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnedit" runat="server" Text="領取獎勵" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3"></TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3"><b>宣傳語(內容玩家可以自己修改，但註冊鏈接不能修改，否則不能推薦)</b></TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3">
			<asp:TextBox id="tbcommendtext" onclick="this.select();" runat="server" Width="444px" TextMode="MultiLine"
				Height="154px"></asp:TextBox></TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3"></TD>
	</TR>
</table>
