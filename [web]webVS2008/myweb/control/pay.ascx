<%@ Control Language="c#" AutoEventWireup="false" Codebehind="pay.ascx.cs" Inherits="web.control.pay" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="528" border="0">
	<TR>
		<TD colspan="3">
			<P><b>大陸玩家購買金幣:</b></P>
			<P>大陸地區在線充值系統，支付成功後，金幣將自動充入玩家帳戶，請充值後自行登陸網站查詢，非常方便。</P>
			<P>請務必記下你的訂單號，特殊情況下可以根據訂單號在GM處查詢</P>
			<P><FONT face="宋体" color="#cc3366"><b>重要提示：</b></FONT></P>
			<P><FONT face="宋体" color="#cc3366"><B>1.請先關掉3721或是其它關閉彈出窗口的插件</B></FONT></P>
			<P><FONT face="宋体" color="#cc3366"><B>2.請非中國大陸地區的玩家不要點生成訂單，非大陸地區是不能充值的</B></FONT></P>
			<P><FONT face="宋体" color="#cc3366"><B></FONT></FONT></B></P>
		</TD>
	</TR>
	<tr>
		<td style="WIDTH: 103px">充值遊戲帳號</td>
		<td style="WIDTH: 141px">
			<asp:TextBox id="tbuserid" runat="server" Width="145px" MaxLength="14"></asp:TextBox></td>
		<td><FONT face="宋体">
				<asp:RequiredFieldValidator id="RequiredFieldValidator1" runat="server" ControlToValidate="tbuserid" ErrorMessage="*"></asp:RequiredFieldValidator>請輸入需要充值的遊戲帳號</FONT></td>
	</tr>
	<tr>
		<td style="WIDTH: 103px; HEIGHT: 3px">充值類型</td>
		<td style="WIDTH: 141px; HEIGHT: 3px">
			<asp:DropDownList id="ddtradetype" runat="server">
				<asp:ListItem Value="10" Selected="True">10元（100金幣）</asp:ListItem>
				<asp:ListItem Value="30">30元（300金幣）</asp:ListItem>
				<asp:ListItem Value="50">50元（500金幣）</asp:ListItem>
				<asp:ListItem Value="100">100元（1000金幣）</asp:ListItem>
				<asp:ListItem Value="200">200元卡（2000金幣）</asp:ListItem>
				<asp:ListItem Value="300">300元卡（3000金幣）</asp:ListItem>
				<asp:ListItem Value="500">500元卡（5000金幣）</asp:ListItem>
				
			</asp:DropDownList></td>
		<td style="HEIGHT: 3px">
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="ddtradetype"></asp:RequiredFieldValidator>請選擇充值類型</td>
	</tr>
	<TR>
		<TD style="WIDTH: 103px; HEIGHT: 24px"><FONT face="宋体">請輸入驗證碼</FONT></TD>
		<TD style="WIDTH: 141px; HEIGHT: 24px"><FONT face="宋体">
				<asp:TextBox id="tbverifycode" MaxLength="14" Width="48px" runat="server"></asp:TextBox><IMG id="imgVerify" style="WIDTH: 56px; CURSOR: hand; HEIGHT: 20px" onclick="this.src=this.src"
					height="20" alt="看不清？點擊更換" src="../VerifyCode.aspx" width="56"></FONT></TD>
		<TD style="WIDTH: 210px" align="left" vAlign="middle"></FONT><FONT face="宋体">
				<asp:requiredfieldvalidator id="Requiredfieldvalidator2" runat="server" ErrorMessage="*" ControlToValidate="tbverifycode"></asp:requiredfieldvalidator>請輸入圖片中的四個數字</FONT></TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" height="17" style="HEIGHT: 17px"><FONT face="宋体">
				<asp:Label id="lbltradeno" runat="server" ForeColor="Red" Font-Bold="True" Visible="False"></asp:Label></FONT></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btncreate" runat="server" Text="生成訂單" CssClass="button_1"></asp:Button>
			<asp:Button id="btnpay" runat="server" Text="前往支付寶充值" Visible="False"></asp:Button>
		</TD>
	</TR>
</table>
<asp:RegularExpressionValidator id="RegularExpressionValidator1" runat="server" ErrorMessage="遊戲帳號格式不正確" ValidationExpression="^[A-Za-z0-9]{4,14}$"
	ControlToValidate="tbuserid"></asp:RegularExpressionValidator>
<br>
<asp:regularexpressionvalidator id="RegularExpressionValidator3" runat="server" ErrorMessage="請輸入四位數字" ControlToValidate="tbverifycode"
	ValidationExpression="^\d{4}$"></asp:regularexpressionvalidator>
