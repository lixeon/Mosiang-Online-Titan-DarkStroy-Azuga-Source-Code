namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpalipay : Page
    {
        protected Button btnclear;
        protected Button btnsearchno;
        protected Button btnsearchuserid;
        protected DataGrid DataGrid1;
        protected DataGrid DataGrid2;
        protected DataGrid DataGrid3;
        protected TextBox tbplayerid;
        protected TextBox tbtradeno;

        private void btnclear_Click(object sender, EventArgs e)
        {
            new DataProviders().ExecuteSql("delete from mhcmember..web_alipay where state=0");
        }

        private void btnsearchno_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbtradeno.Text.ToString().Trim());
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_alipay where tradeno= '" + str + "' order by date desc", "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void btnsearchuserid_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbplayerid.Text.ToString().Trim());
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_alipay where userid= '" + str + "' order by date desc", "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void DataGrid1_Delete(object sender, DataGridCommandEventArgs e)
        {
            string text = e.Item.Cells[0].Text;
            new DataProviders().ExecuteSql("delete from mhcmember..web_alipay where tradeno='" + text + "'");
        }

        private void DataGrid1_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid1.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_alipay order by date desc", "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void DataGrid2_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid2.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid2.DataSource = new DataProviders().ExecuteSqlDs("select convert(char(10),date,120) as date,sum(money) as money from mhcmember..web_alipay where state=1 group by convert(char(10),date,120) order by date desc", "DataGrid2");
            this.DataGrid2.DataBind();
        }

        private void InitializeComponent()
        {
            this.DataGrid1.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid1_PageIndexChanged);
            this.DataGrid1.DeleteCommand += new DataGridCommandEventHandler(this.DataGrid1_Delete);
            this.DataGrid2.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid2_PageIndexChanged);
            this.btnsearchuserid.Click += new EventHandler(this.btnsearchuserid_Click);
            this.btnclear.Click += new EventHandler(this.btnclear_Click);
            this.btnsearchno.Click += new EventHandler(this.btnsearchno_Click);
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
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_alipay order by date desc", "DataGrid1");
                this.DataGrid1.DataBind();
                this.DataGrid2.DataSource = new DataProviders().ExecuteSqlDs("select convert(char(10),date,120) as date,sum(money) as money from mhcmember..web_alipay where state=1 group by convert(char(10),date,120) order by date desc", "DataGrid2");
                this.DataGrid2.DataBind();
                this.DataGrid3.DataSource = new DataProviders().ExecuteSqlDs("select convert(char(7),date,120) as date,sum(money) as money  from mhcmember..web_alipay where state=1  group by convert(char(7),date,120) order by date desc", "DataGrid3");
                this.DataGrid3.DataBind();
            }
        }
    }
}
