namespace web.admin
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpitem : Page
    {
        protected Button btnadd;
        protected Button btnedit;
        protected DataGrid DataGrid1;
        protected DropDownList DropDownList1;
        protected DropDownList Dropdownlist2;
        protected DropDownList DropDownList3;
        protected Label lblid;
        protected RequiredFieldValidator RequiredFieldValidator2;
        protected TextBox tbcomment;
        protected TextBox tbgold;
        protected TextBox tbitemid;
        protected TextBox tbname;
        protected TextBox tbpic;
        protected TextBox tbprice;

        private void btnadd_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbname.Text.ToString());
            int num = int.Parse(this.tbitemid.Text);
            int num2 = int.Parse(this.DropDownList1.SelectedValue);
            int num3 = int.Parse(this.DropDownList3.SelectedValue);
            int num4 = int.Parse(this.tbprice.Text.ToString());
            int num5 = int.Parse(this.tbgold.Text.ToString());
            string str2 = new system().ChkSql(this.tbcomment.Text.ToString());
            string str3 = new system().ChkSql(this.tbpic.Text.ToString());
            new DataProviders().ExecuteSql(string.Concat(new object[] { 
                "insert into web_item (name,itemid,comment,pic,type,price,gold,num) values ('", str, "',", num, ",'", str2, "','", str3, "',", num2, ",", num4, ",", num5, ",", num3, 
                ")"
             }));
            int num6 = int.Parse(this.Dropdownlist2.SelectedValue);
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select a.id as id ,a.name,a.itemid,b.name as type,price,gold,buynum from web_item a,web_itemtype b where a.type=b.id and a.type=" + num6, "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void btnedit_Click(object sender, EventArgs e)
        {
            int num = int.Parse(this.lblid.Text);
            string str = new system().ChkSql(this.tbname.Text.ToString());
            int num2 = int.Parse(this.tbitemid.Text);
            int num3 = int.Parse(this.DropDownList1.SelectedValue);
            int num4 = int.Parse(this.DropDownList3.SelectedValue);
            int num5 = int.Parse(this.tbprice.Text.ToString());
            int num6 = int.Parse(this.tbgold.Text.ToString());
            string str2 = new system().ChkSql(this.tbcomment.Text.ToString());
            string str3 = new system().ChkSql(this.tbpic.Text.ToString());
            new DataProviders().ExecuteSql(string.Concat(new object[] { 
                "update web_item set name='", str, "',itemid=", num2, ",comment='", str2, "',pic='", str3, "',type=", num3, ",price=", num5, ",gold=", num6, ",num=", num4, 
                " where id=", num
             }));
            int num7 = int.Parse(this.Dropdownlist2.SelectedValue);
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select a.id as id ,a.name,a.itemid,b.name as type,price,gold,buynum from web_item a,web_itemtype b where a.type=b.id and a.type=" + num7, "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void DataGrid1_Delete(object sender, DataGridCommandEventArgs e)
        {
            int num = int.Parse(e.Item.Cells[0].Text);
            new DataProviders().ExecuteSql("delete from web_item where id=" + num);
        }

        private void DataGrid1_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            int num = int.Parse(this.Dropdownlist2.SelectedValue);
            string str = "";
            if (num != 0)
            {
                str = " and a.type=" + num;
            }
            this.DataGrid1.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select a.id as id ,a.itemid,a.name,b.name as type,price,gold,buynum from web_item a,web_itemtype b where a.type=b.id" + str, "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void DataGrid1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from mhcmember..web_item where id=" + num);
            if (reader.Read())
            {
                this.lblid.Text = num.ToString();
                this.tbname.Text = reader["name"].ToString();
                this.tbitemid.Text = reader["itemid"].ToString();
                this.tbprice.Text = reader["price"].ToString();
                this.tbgold.Text = reader["gold"].ToString();
                this.tbpic.Text = reader["pic"].ToString();
                this.tbcomment.Text = reader["comment"].ToString();
                this.DropDownList1.SelectedValue = reader["type"].ToString();
                this.DropDownList3.SelectedValue = reader["num"].ToString();
                this.btnadd.Visible = true;
                this.btnedit.Visible = true;
            }
            reader.Close();
            providers.CloseConn();
        }

        private void Dropdownlist2_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.Dropdownlist2.SelectedValue);
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select a.id as id ,a.name,a.itemid,b.name as type,price,gold,buynum from web_item a,web_itemtype b where a.type=b.id and a.type=" + num, "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void InitializeComponent()
        {
            this.Dropdownlist2.SelectedIndexChanged += new EventHandler(this.Dropdownlist2_SelectedIndexChanged);
            this.DataGrid1.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid1_PageIndexChanged);
            this.DataGrid1.DeleteCommand += new DataGridCommandEventHandler(this.DataGrid1_Delete);
            this.DataGrid1.SelectedIndexChanged += new EventHandler(this.DataGrid1_SelectedIndexChanged);
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            this.btnadd.Click += new EventHandler(this.btnadd_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isadmin();
            new WebLogic().diskf();
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select a.id as id ,a.itemid,a.name,b.name as type,price,gold,buynum from web_item a,web_itemtype b where a.type=b.id", "DataGrid1");
                this.DataGrid1.DataBind();
                this.DropDownList1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_itemtype", "DropDownList1");
                this.DropDownList1.DataTextField = "name";
                this.DropDownList1.DataValueField = "id";
                this.DropDownList1.DataBind();
                this.Dropdownlist2.DataSource = new DataProviders().ExecuteSqlDs("select * from web_itemtype", "DropDownList2");
                this.Dropdownlist2.DataTextField = "name";
                this.Dropdownlist2.DataValueField = "id";
                this.Dropdownlist2.DataBind();
                this.Dropdownlist2.Items.Insert(0, new ListItem("請選擇", "0"));
            }
        }
    }
}
