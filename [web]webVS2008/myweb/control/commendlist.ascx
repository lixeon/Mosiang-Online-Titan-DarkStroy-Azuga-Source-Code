<%@ Control Language="c#" AutoEventWireup="false" Codebehind="commendlist.ascx.cs" Inherits="web.control.commendlist" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<FONT face="宋体">
	<asp:DataGrid id="DataGrid1" runat="server" AutoGenerateColumns="False" Width="100%" BorderColor="#CC9966"
		BorderStyle="None" BorderWidth="1px" BackColor="White" CellPadding="4">
		<SelectedItemStyle Font-Bold="True" ForeColor="#663399" BackColor="#FFCC66"></SelectedItemStyle>
		<ItemStyle ForeColor="#330099" BackColor="White"></ItemStyle>
		<HeaderStyle Font-Bold="True" ForeColor="#FFFFCC" BackColor="#990000"></HeaderStyle>
		<FooterStyle ForeColor="#330099" BackColor="#FFFFCC"></FooterStyle>
		<Columns>
			<asp:TemplateColumn>
				<HeaderTemplate>
					<center>排名</center>
				</HeaderTemplate>
				<ItemTemplate>
					<center><%=i++%></center>
				</ItemTemplate>
			</asp:TemplateColumn>
			<asp:TemplateColumn>
				<HeaderTemplate>
					<center>角色名</center>
				</HeaderTemplate>
				<ItemTemplate>
					<center><%# sys.ConvertToBig5((DataBinder.Eval(Container.DataItem,"Character_Name").ToString()),950)%></center>
				</ItemTemplate>
			</asp:TemplateColumn>
			<asp:BoundColumn DataField="webcommendgift" HeaderText="已領取獎勵數"></asp:BoundColumn>
			<asp:BoundColumn DataField="webcommendidnum" HeaderText="推薦總人數"></asp:BoundColumn>
		</Columns>
		<PagerStyle HorizontalAlign="Center" ForeColor="#330099" BackColor="#FFFFCC"></PagerStyle>
	</asp:DataGrid></FONT>
