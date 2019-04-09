<%@ Control Language="c#" AutoEventWireup="false" Codebehind="clearpk.ascx.cs" Inherits="web.control.clearpk" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="528" border="0">
	<TR>
		<TD colspan="3">
			<P><b>清洗惡名:</b></P>
			<P>清洗惡名需要退出遊戲</P>
			<p>洗一點惡名需要金幣<b><font color="#ff3333"><%=Application["game.clearpkgold"]%></font>個</b></p>
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
			<asp:DropDownList id="ddchalist" runat="server"></asp:DropDownList></td>
		<td style="HEIGHT: 20px">
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="ddchalist"></asp:RequiredFieldValidator>請選擇需要清洗惡名的角色</td>
	</tr>
	<TR>
		<TD style="WIDTH: 103px; HEIGHT: 20px"><FONT face="細明體">清洗數</FONT></TD>
		<TD style="WIDTH: 136px; HEIGHT: 20px">
			<asp:TextBox id="tbpk" runat="server" MaxLength="14" Width="88px"></asp:TextBox></TD>
		<TD style="HEIGHT: 20px">
			<asp:RequiredFieldValidator id="RequiredFieldValidator1" runat="server" ControlToValidate="tbpk" ErrorMessage="*"></asp:RequiredFieldValidator>請輸入需要清洗的惡名數</TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" height="10"><FONT face="宋体"></FONT></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnclear" runat="server" Text="清洗惡名" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>
<asp:RegularExpressionValidator id="RegularExpressionValidator1" runat="server" ErrorMessage="請填寫正整數" ValidationExpression="^\+?[1-9][0-9]*$"
	ControlToValidate="tbpk"></asp:RegularExpressionValidator>
