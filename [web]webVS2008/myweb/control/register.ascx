<%@ Control Language="c#" AutoEventWireup="false" Codebehind="register.ascx.cs" Inherits="web.control.register" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table id="Table1" cellSpacing="1" cellPadding="1" width="512" border="0" style="WIDTH: 512px; HEIGHT: 348px">
	<TR align="left">
		<TD colSpan="3"><strong>遊戲帳號註冊</strong></TD>
	</TR>
	<tr>
		<td>登陸帳號</td>
		<td><asp:textbox id="tbuserid" runat="server"></asp:textbox></td>
		<td align="left"><asp:requiredfieldvalidator id="RequiredFieldValidator1" runat="server" ControlToValidate="tbuserid" ErrorMessage="*"></asp:requiredfieldvalidator>僅限4~12位a-z,A-Z字母及數字</td>
	</tr>
	<TR>
		<TD class="red" colSpan="3">注意：為了您的帳號安全，輸入的帳號請不要與論壇帳號或遊戲中角色名相同</TD>
	</TR>
	<tr>
		<td style="HEIGHT: 26px">密碼</td>
		<td style="HEIGHT: 26px"><asp:textbox id="tbuserpwd" runat="server" TextMode="Password"></asp:textbox></td>
		<td align="left" style="HEIGHT: 26px"><asp:requiredfieldvalidator id="RequiredFieldValidator2" runat="server" ControlToValidate="tbuserpwd" ErrorMessage="*"></asp:requiredfieldvalidator>僅限4~12位a-z,A-Z字母及數字</td>
	</tr>
	<tr>
		<td>確認密碼</td>
		<td><asp:textbox id="tbreuserpwd" runat="server" TextMode="Password"></asp:textbox></td>
		<td align="left">僅限4~12位a-z,A-Z字母及數字</td>
	</tr>
	<TR>
		<TD>性別</TD>
		<TD><asp:radiobutton id="rbmale" runat="server" Checked="True" Text="男" GroupName="reg"></asp:radiobutton><asp:radiobutton id="rbfemale" runat="server" Text="女" GroupName="reg"></asp:radiobutton></TD>
		<TD align="left">請選擇性別</TD>
	</TR>
	<TR>
		<TD>電子郵箱</TD>
		<TD><asp:textbox id="tbemail" runat="server"></asp:textbox></TD>
		<TD align="left"><asp:requiredfieldvalidator id="RequiredFieldValidator3" runat="server" ControlToValidate="tbemail" ErrorMessage="*"></asp:requiredfieldvalidator>請輸入郵箱地址</TD>
	</TR>
	<TR>
		<TD>二級密碼</TD>
		<TD align="left"><asp:textbox id="tbkey" runat="server" TextMode="Password"></asp:textbox></TD>
		<TD align="left"><asp:requiredfieldvalidator id="Requiredfieldvalidator4" runat="server" ControlToValidate="tbkey" ErrorMessage="*"
				Display="Dynamic"></asp:requiredfieldvalidator>僅限4~12位a-z,A-Z字母及數字
		</TD>
	</TR>
	<TR>
		<TD class="red" colSpan="3">注意：二級密碼不能與登陸帳號或登陸密碼一樣，是以後找回密碼的唯一依據</TD>
	</TR>
	<TR>
		<TD style="HEIGHT: 15px">推薦人</TD>
		<TD style="HEIGHT: 15px"><asp:textbox id="tbcommend" runat="server" Columns="10" BackColor="#E0E0E0" ReadOnly="True"></asp:textbox></TD>
		<TD align="left" style="HEIGHT: 15px">沒有推薦人請留空</TD>
	</TR>
	<TR>
		<TD>驗證碼</TD>
		<TD><asp:textbox id="tbverifycode" runat="server" Columns="4" MaxLength="4"></asp:textbox><IMG id="imgVerify" style="WIDTH: 56px; CURSOR: hand; HEIGHT: 20px" onclick="this.src=this.src"
				height="20" alt="看不清？點擊更換" src="./VerifyCode.aspx" width="56"></TD>
		<TD><asp:requiredfieldvalidator id="Requiredfieldvalidator5" runat="server" ControlToValidate="tbverifycode" ErrorMessage="*"></asp:requiredfieldvalidator>請輸入圖片中的四位數字</TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" height="5"></TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3"><A class="red" href="agreement.aspx" target="_blank">確認註冊表示您已經仔細閱讀了《用戶協議》</A></TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" height="5"></TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3"><asp:button id="btnregister" runat="server" Text="注  冊" CssClass="button_1"></asp:button></TD>
	</TR>
</table>
<P><asp:regularexpressionvalidator id="RegularExpressionValidator1" runat="server" ControlToValidate="tbuserid" ErrorMessage="帳號格式錯誤!"
		ValidationExpression="^[A-Za-z0-9]{4,12}$"></asp:regularexpressionvalidator><br>
	<asp:regularexpressionvalidator id="RegularExpressionValidator2" runat="server" ControlToValidate="tbuserpwd" ErrorMessage="密碼格式錯誤!"
		ValidationExpression="^[A-Za-z0-9]{4,12}$"></asp:regularexpressionvalidator><br>
	<asp:comparevalidator id="CompareValidator1" runat="server" ControlToValidate="tbreuserpwd" ErrorMessage="兩次密碼不一致!"
		ControlToCompare="tbuserpwd"></asp:comparevalidator><br>
	<asp:RegularExpressionValidator id="RegularExpressionValidator7" runat="server" ControlToValidate="tbemail" ErrorMessage="郵箱格式錯誤!"
		ValidationExpression="\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*"></asp:RegularExpressionValidator><br>
	<asp:RegularExpressionValidator id="RegularExpressionValidator4" runat="server" ErrorMessage="二級密碼格式錯誤!" ControlToValidate="tbkey"
		ValidationExpression="^[A-Za-z0-9]{4,12}$"></asp:RegularExpressionValidator><br>
	<asp:RegularExpressionValidator id="RegularExpressionValidator6" runat="server" ErrorMessage="驗證碼格式錯誤!" ControlToValidate="tbverifycode"
		ValidationExpression="\d{4}$"></asp:RegularExpressionValidator><br>
	<asp:RegularExpressionValidator id="RegularExpressionValidator5" runat="server" ErrorMessage="推薦人格式錯誤!" ControlToValidate="tbcommend"
		ValidationExpression="^[A-Za-z0-9]{4,12}$"></asp:RegularExpressionValidator><br>
</P>
