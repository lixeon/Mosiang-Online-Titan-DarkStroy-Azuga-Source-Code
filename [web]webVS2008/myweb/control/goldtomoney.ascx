<%@ Control Language="c#" AutoEventWireup="false" Codebehind="goldtomoney.ascx.cs" Inherits="web.control.goldtomoney" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="528" border="0">
	<TR>
		<TD colspan="3">
			<P><b>購買遊戲幣:</b></P>
			<P>購買遊戲幣不需要退出遊戲</P>
			<P>購買成功後遊戲幣將放入你的網上銀行,請登陸網上銀行領取遊戲幣</P>
			<p>每1個金幣可以換取<b><font color="#ff3333"><%=Application["game.goldtomoney"]%></font>遊戲幣</b></p>
			<P><FONT face="宋体"></FONT>&nbsp;</P>
		</TD>
	</TR>
	<tr>
		<td style="WIDTH: 76px">登陸帳號</td>
		<td style="WIDTH: 136px"><strong><%=Session["userid"]%></strong></td>
		<td><FONT face="宋体"></FONT></td>
	</tr>
	<TR>
		<TD style="WIDTH: 76px; HEIGHT: 20px"><FONT face="細明體">金幣</FONT></TD>
		<TD style="WIDTH: 136px; HEIGHT: 20px">
			<asp:TextBox id="tbgold" runat="server" MaxLength="14" Width="88px"></asp:TextBox></TD>
		<TD style="HEIGHT: 20px">
			<asp:RequiredFieldValidator id="RequiredFieldValidator1" runat="server" ControlToValidate="tbgold" ErrorMessage="*"></asp:RequiredFieldValidator>請輸入需要兌換的金幣數量</TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" height="10"><FONT face="宋体"></FONT></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnchange" runat="server" Text="購買遊戲幣" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>
<asp:RegularExpressionValidator id="RegularExpressionValidator1" runat="server" ErrorMessage="請填寫正整數" ValidationExpression="^\+?[1-9][0-9]*$"
	ControlToValidate="tbgold"></asp:RegularExpressionValidator>
