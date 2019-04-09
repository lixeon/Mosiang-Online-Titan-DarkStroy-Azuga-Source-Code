<%@ Control Language="c#" AutoEventWireup="false" Codebehind="skilllevelup.ascx.cs" Inherits="web.control.skilllevelup" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="528" border="0">
	<TR>
		<TD colspan="3">
			<P><b>升級武功及陣法:</b></P>
			<P>升級武功及陣法需要退出遊戲</P>
			<p>每升一級武功或陣法需要遊戲幣<b><font color="#ff3333"><%=Application["game.skilllvupmoney"]%></font></b>,最高可以升到12級</p>
			<P>&nbsp;</P>
		</TD>
	</TR>
	<tr>
		<td style="WIDTH: 103px">登陸帳號</td>
		<td style="WIDTH: 136px"><strong><%=Session["userid"]%></strong></td>
		<td><FONT face="宋体"></FONT></td>
	</tr>
	<tr>
		<td style="WIDTH: 103px; HEIGHT: 20px">角色列表</td>
		<td style="WIDTH: 136px; HEIGHT: 20px">
			<asp:DropDownList id="ddchalist" runat="server" AutoPostBack="True"></asp:DropDownList></td>
		<td style="HEIGHT: 20px">
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="ddchalist"></asp:RequiredFieldValidator>請選擇需要升級武功的角色</td>
	</tr>
	<TR>
		<TD style="WIDTH: 103px; HEIGHT: 20px">武功及陣法</TD>
		<TD style="WIDTH: 136px; HEIGHT: 20px">
			<asp:DropDownList id="ddmugong" runat="server"></asp:DropDownList></TD>
		<TD style="HEIGHT: 20px">
			<asp:RequiredFieldValidator id="RequiredFieldValidator1" runat="server" ControlToValidate="ddmugong" ErrorMessage="*"></asp:RequiredFieldValidator>請選擇需要升級的武功名稱</TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" height="10"><FONT face="宋体"></FONT></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnlvup" runat="server" Text="修煉武功" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>
