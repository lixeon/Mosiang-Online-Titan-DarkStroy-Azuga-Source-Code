<%@ Control Language="c#" AutoEventWireup="false" Codebehind="hero.ascx.cs" Inherits="web.control.hero" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>

<asp:datagrid id="DataGrid1" runat="server" AutoGenerateColumns="False" Width="100%" BorderColor="#CC9966"
	BorderStyle="None" BorderWidth="1px" BackColor="White" CellPadding="4" Height="160px">
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
				<center><%# sys.ConvertToBig5((DataBinder.Eval(Container.DataItem,"character_name").ToString()),950)%></center>
			</ItemTemplate>
		</asp:TemplateColumn>
		<asp:BoundColumn DataField="character_grade" HeaderText="等級"></asp:BoundColumn>
		<asp:BoundColumn DataField="webchareset" HeaderText="轉生"></asp:BoundColumn>
		<asp:BoundColumn DataField="character_money" HeaderText="持有金"></asp:BoundColumn>
		<asp:BoundColumn DataField="character_fame" HeaderText="聲望"></asp:BoundColumn>
		<asp:BoundColumn DataField="character_badfame" HeaderText="惡名"></asp:BoundColumn>
	</Columns>
	<PagerStyle HorizontalAlign="Center" ForeColor="#330099" BackColor="#FFFFCC"></PagerStyle>
</asp:datagrid>
