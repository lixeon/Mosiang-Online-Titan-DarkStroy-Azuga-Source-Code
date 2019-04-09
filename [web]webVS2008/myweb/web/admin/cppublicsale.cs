namespace web.admin
{
    using FredCK.FCKeditorV2;
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cppublicsale : Page
    {
        protected Button btnadd;
        protected Button btnedit;
        protected CheckBox cbdate;
        protected DataGrid DataGrid1;
        protected DropDownList ddisend;
        protected FCKeditor edit;
        protected Label lblid;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected TextBox tbpsdate;
        protected TextBox tbtitle;
        protected TextBox tbwinprice;
        protected TextBox tbwinuserid;

        private void btnadd_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbtitle.Text.ToString());
            int num = int.Parse(this.ddisend.SelectedValue);
            int num2 = int.Parse(this.tbpsdate.Text.ToString());
            string str2 = new system().ChkSql(this.tbwinuserid.Text.ToString());
            int num3 = int.Parse(new system().ChkSql(this.tbwinprice.Text.ToString()));
            string str3 = new system().ChkSql(this.edit.Value.ToString());
            new DataProviders().ExecuteSql(string.Concat(new object[] { "insert into web_publicsale (title,content,winuserid,winprice,adddate,enddate,isend) values ('", str, "','", str3, "','", str2, "',", num3, ",getdate(),getdate()+", num2, ",", num, ")" }));
            base.Response.Redirect("cppublicsale.aspx");
        }

        private void btnedit_Click(object sender, EventArgs e)
        {
            int num = int.Parse(this.lblid.Text);
            this.tbpsdate.Text = "0";
            string str = new system().ChkSql(this.tbtitle.Text.ToString());
            string str2 = new system().ChkSql(this.edit.Value.ToString());
            string str3 = new system().ChkSql(this.tbwinuserid.Text.ToString());
            string str4 = new system().ChkSql(this.tbwinprice.Text.ToString());
            int num2 = int.Parse(this.ddisend.SelectedValue);
            int num3 = int.Parse(new system().ChkSql(this.tbpsdate.Text.ToString()));
            string str5 = this.cbdate.Checked ? (",enddate=getdate()+" + num3) : "";
            string mySql = string.Concat(new object[] { "update web_publicsale set title='", str, "',content='", str2, "',winuserid='", str3, "',winprice=", str4, ",isend=", num2, str5, " where id=", num });
            new DataProviders().ExecuteSql(mySql);
            this.btnedit.Visible = false;
            this.cbdate.Visible = false;
            this.btnadd.Visible = true;
            base.Response.Redirect("cppublicsale.aspx");
        }

        private void DataGrid1_Delete(object sender, DataGridCommandEventArgs e)
        {
            int psid = int.Parse(e.Item.Cells[0].Text);
            string str = new WebLogic().stoppublicsale(psid);
            base.Response.Write("<script>alert('" + str + "');</script>");
        }

        private void DataGrid1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from web_publicsale where id=" + num);
            if (reader.Read())
            {
                this.lblid.Text = num.ToString();
                this.tbtitle.Text = reader["title"].ToString();
                this.edit.Value = reader["content"].ToString();
                this.tbwinuserid.Text = reader["winuserid"].ToString();
                this.tbwinprice.Text = reader["winprice"].ToString();
                this.ddisend.SelectedValue = reader["isend"].ToString();
                this.btnadd.Visible = false;
                this.btnedit.Visible = true;
                this.cbdate.Visible = true;
                this.btnadd.Visible = true;
            }
            reader.Close();
            providers.CloseConn();
        }

        private void InitializeComponent()
        {
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
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_publicsale order by adddate desc", "DataGrid1");
                this.DataGrid1.DataBind();
            }
        }
    }
}
