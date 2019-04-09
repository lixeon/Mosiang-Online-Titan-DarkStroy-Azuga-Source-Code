<%@ Control Language="c#" AutoEventWireup="false" Codebehind="resetpoint.ascx.cs" Inherits="web.control.resetpoint" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="520" border="0" style="WIDTH: 520px; HEIGHT: 228px">
	<TR>
		<TD colspan="3">
			<P><b>在線洗點:</b></P>
			<P>在線洗點需要退出遊戲</P>
			<p>洗點需要金幣<b><font color="#ff3333"><%=Application["game.resetpointgold"]%></font>個</b>,遊戲幣<b><font color="#ff3333"><%=Application["game.resetpointmoney"]%></font>元</b></p>
			<P>注意：15轉以上的玩家洗點後新轉轉數屬性自動加至體質，具體請參考轉生注意事項</P>
		</TD>
	</TR>
	<tr>
		<td style="WIDTH: 103px">登陸帳號</td>
		<td style="WIDTH: 120px"><strong><%=Session["userid"]%></strong></td>
		<td><FONT face="宋体"></FONT></td>
	</tr>
	<tr>
		<td style="WIDTH: 103px; HEIGHT: 18px">角色列表</td>
		<td style="WIDTH: 120px; HEIGHT: 18px">
			<asp:DropDownList id="ddchalist" runat="server"></asp:DropDownList></td>
		<td style="HEIGHT: 18px">
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="ddchalist"></asp:RequiredFieldValidator>請選擇需要洗點的角色</td>
	</tr>
	<TR>
		<TD align="center" colSpan="3" height="10"><FONT face="宋体"></FONT></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnreset" runat="server" Text="確認洗點" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>
