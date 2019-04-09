<%@ Control Language="c#" AutoEventWireup="false" Codebehind="modifychaname.ascx.cs" Inherits="web.control.modifychaname" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="528" border="0">
	<TR>
		<TD colspan="3">
			<P><b>角色改名:</b></P>
			<P>改名需要退出遊戲，最多可以改<span style="FONT-WEIGHT:bold;COLOR:#5D51F1"><%=Application["game.modifychanamecount"]%></span>次名 注：不可以使用^%&amp;',;=?$\等字符</P>
			<p>每次改名需要花費遊戲幣<span style="FONT-WEIGHT:bold;COLOR:#FF3333"><%=Application["game.modifychanamemoney"]%></span>元，請把錢放物品欄再來改名</p>
		</TD>
	</TR>
	<tr>
		<td style="WIDTH: 103px">登陸帳號</td>
		<td style="WIDTH: 120px"><strong><%=Session["userid"]%></strong></td>
		<td></td>
	</tr>
	<tr>
		<td style="WIDTH: 103px; HEIGHT: 20px">角色列表</td>
		<td style="WIDTH: 120px; HEIGHT: 20px">
			<asp:DropDownList id="ddchalist" runat="server"></asp:DropDownList></td>
		<td style="HEIGHT: 20px">
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="ddchalist"></asp:RequiredFieldValidator>請選擇需要改名的角色</td>
	</tr>
	<TR>
		<TD style="WIDTH: 103px; HEIGHT: 20px"><FONT face="細明體">新角色名</FONT></TD>
		<TD style="WIDTH: 120px; HEIGHT: 20px">
			<asp:TextBox id="tbnewname" runat="server" MaxLength="14" Width="145px"></asp:TextBox></TD>
		<TD style="HEIGHT: 20px">
				<asp:RequiredFieldValidator id="RequiredFieldValidator1" runat="server" ControlToValidate="tbnewname" ErrorMessage="*"></asp:RequiredFieldValidator>請輸入新角色名</FONT></TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" height="10"></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnedit" runat="server" Text="修改角色名稱" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>
<asp:RegularExpressionValidator id="RegularExpressionValidator1" runat="server" ErrorMessage="角色名格式不正確" ValidationExpression="^[^%&amp;',;=?$\x22]+"
	ControlToValidate="tbnewname"></asp:RegularExpressionValidator>
