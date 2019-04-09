<%@ Page language="c#" Codebehind="admincp.aspx.cs" AutoEventWireup="false" Inherits="web.admin.admincp" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
	<HEAD>
		<title>admincp</title>
		<meta content="Microsoft Visual Studio .NET 7.1" name="GENERATOR">
		<meta content="C#" name="CODE_LANGUAGE">
		<meta content="JavaScript" name="vs_defaultClientScript">
		<meta content="http://schemas.microsoft.com/intellisense/ie5" name="vs_targetSchema">
		<LINK href="Common/CSS/Default.CSS" type="text/css" rel="stylesheet">
		<LINK href="Common/CSS/menuStyleXP.CSS" type="text/css" rel="stylesheet">
	</HEAD>
	<body MS_POSITIONING="GridLayout">
		<FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体">
		</FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体">
		</FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体">
		</FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体">
		</FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体">
		</FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体">
		</FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT><FONT face="宋体">
		</FONT><FONT face="宋体"></FONT><FONT face="宋体"></FONT>
		<br>
		<br>
		<br>
		<br>
		<br>
		<br>
		<form id="Form1" method="post" runat="server">
			<table cellSpacing="1" cellPadding="1" width="50%" align="center">
				<tr>
					<td vAlign="middle" align="center"><br>
						<br>
						<b>請輸入您的用戶名和密碼 </b>
						<br>
						<br>
						<table cellSpacing="0" cellPadding="0" width="40%" align="center">
							<tr>
								<td>
									<table style="WIDTH: 424px; HEIGHT: 134px" cellSpacing="0" cellPadding="3" width="40%">
										<tr>
											<td style="WIDTH: 98px" vAlign="middle" align="left" width="98">請輸入管理員ID
											</td>
											<td style="WIDTH: 210px; HEIGHT: 29px" vAlign="middle"><input id="admin_id" type="text" size="20" name="admin_id" runat="server">
											</td>
											<TD style="WIDTH: 85px; HEIGHT: 29px" vAlign="middle"><asp:regularexpressionvalidator id="RegularExpressionValidator1" runat="server" ValidationExpression="^[A-Za-z0-9]{4,12}$"
													ControlToValidate="admin_id" ErrorMessage="*"></asp:regularexpressionvalidator><asp:requiredfieldvalidator id="RequiredFieldValidator3" runat="server" ControlToValidate="admin_id" ErrorMessage="*"></asp:requiredfieldvalidator></TD>
										</tr>
										<tr>
											<td style="WIDTH: 98px; HEIGHT: 33px" vAlign="middle" align="left" width="98">請輸入管理員密碼
											</td>
											<td style="WIDTH: 210px; HEIGHT: 33px" vAlign="middle"><input id="admin_pwd" type="password" size="21" name="admin_pwd" runat="server">
											</td>
											<TD style="WIDTH: 85px; HEIGHT: 33px" vAlign="middle"><asp:requiredfieldvalidator id="RequiredFieldValidator2" runat="server" ControlToValidate="admin_pwd" ErrorMessage="*"></asp:requiredfieldvalidator></TD>
										</tr>
										<tr>
											<td style="WIDTH: 98px" vAlign="middle" align="left" width="98">請輸入驗證碼
											</td>
											<td style="WIDTH: 210px" vAlign="middle" align="left"><input id="vcode" type="text" maxLength="4" size="4" name="vcode" runat="server">
												<asp:regularexpressionvalidator id="RegularExpressionValidator3" runat="server" ValidationExpression="^\d{4}$" ControlToValidate="vcode"
													ErrorMessage="*"></asp:regularexpressionvalidator><asp:requiredfieldvalidator id="RequiredFieldValidator1" runat="server" ControlToValidate="vcode" ErrorMessage="*"></asp:requiredfieldvalidator><IMG id="imgVerify" style="WIDTH: 56px; CURSOR: hand; HEIGHT: 20px" align=absmiddle onclick="this.src=this.src"
													height="20" alt="看不清？點擊更換" src="../VerifyCode.aspx" width="56"></td>
											<TD style="WIDTH: 85px" vAlign="middle"><FONT face="宋体"></FONT></TD>
										</tr>
										<TR vAlign="middle">
											<td vAlign="middle" align="center" colSpan="3"><input id="btnLogin" type="submit" value="登 陸" name="submit" runat="server">
											</td>
										</TR>
									</table>
								</td>
							</tr>
						</table>
						<br>
					</td>
				</tr>
			</table>
		</form>
	</body>
</HTML>
