<%@ Control Language="c#" AutoEventWireup="false" Codebehind="petlevelup.ascx.cs" Inherits="web.control.petlevelup" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="528" border="0">
	<TR>
		<TD colspan="3" style="HEIGHT: 290px">
			<P><b>寵物升級:</b></P>
			<P>寵物升級時玩家需要退出遊戲，寵物最高可以升到<FONT color="#0000ff"><B>3</B></FONT>級：</P>
			<P>1.一級寵物升二級寵物需要金幣<span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=Application["game.petlvupgold2"]%></span></P>
			<P>2.二級寵物升三級寵物需要金幣<span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=Application["game.petlvupgold3"]%></span></P>
			<P>說明:升級成功率為<FONT color="#0000ff"><STRONG>100%</STRONG></FONT><FONT color="#000000">,升級後寵物的親密度為<FONT color="#0000ff"><STRONG>30</STRONG></FONT></FONT></P>
		</TD>
	</TR>
	<tr>
		<td style="WIDTH: 103px">登陸帳號</td>
		<td style="WIDTH: 120px"><strong><%=Session["userid"]%></strong></td>
		<td><FONT face="宋体"></FONT></td>
	</tr>
	<tr>
		<td style="WIDTH: 103px; HEIGHT: 14px">寵物列表</td>
		<td style="WIDTH: 120px; HEIGHT: 14px">
			<asp:DropDownList id="ddpetlist" runat="server"></asp:DropDownList></td>
		<td style="HEIGHT: 14px">
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="ddpetlist"></asp:RequiredFieldValidator>請選擇需要升級的寵物</td>
	</tr>
	<TR>
		<TD align="center" colSpan="3" height="10"><FONT face="宋体"></FONT></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnedit" runat="server" Text="確認升級" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>
